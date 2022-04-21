import numpy
import open3d
import os.path

# Credits to Florent Poux at https://towardsdatascience.com/5-step-guide-to-generate-3d-meshes-from-point-clouds-with-python-36bad397d8ba

def main():
    # Establish path to files
    inputPath = "pointClouds/input/"
    outputPath = "pointClouds/output/"
    inputName = "sample_w_normals.xyz"
    outputName = "scene.gltf"
    if not os.path.exists(inputPath + inputName):
        print("Failed to find file " + inputPath + inputName + ".")
        return
    
    # Load in file
    pointCloud = numpy.loadtxt(inputPath + inputName, skiprows=1)

    # Transform data type of numpy point cloud to something usable in Open3D
    # We add points, colours, normals from the numpy list
    # Remember in openGL we are passing each vertex as a vec3 of coords, colours, normals
    # Here you can see the data type starts at different offsets, 0, 3, 6
    pcd = open3d.geometry.PointCloud()
    pcd.points = open3d.utility.Vector3dVector(pointCloud[:,:3])
    pcd.colors = open3d.utility.Vector3dVector(pointCloud[:,3:6]/255)
    pcd.normals = open3d.utility.Vector3dVector(pointCloud[:,6:9])

    # Ball Pivoting Algorithm (BPA)
    # We place a sphere that is of slightly larger diameter than the average space between points.
    # In this way we find a base triangle of which we can form more triangles from 2 adjacent edges.
    # The ball radius is chosen by the size of the pcd.
    distances = pcd.compute_nearest_neighbor_distance()
    avgDis = numpy.mean(distances)
    radius = 3 * avgDis

    # Create the mesh
    bpaMesh = open3d.geometry.TriangleMesh.create_from_point_cloud_ball_pivoting(
        pcd, open3d.utility.DoubleVector([radius, radius * 2]))

    # Downsample result to less triangles
    compressedMesh = bpaMesh.simplify_quadric_decimation(100000)

    # Get rid of artifacts
    compressedMesh.remove_degenerate_triangles()
    compressedMesh.remove_duplicated_triangles()
    compressedMesh.remove_duplicated_vertices()
    compressedMesh.remove_non_manifold_edges()

    # Export the mesh to file
    open3d.io.write_triangle_mesh(outputPath + outputName, compressedMesh)

    # Save the file at different levels of detail as well
    meshLods = generateLevelsOfDetails(bpaMesh, [1000, 100], ".gltf", outputPath)
    open3d.visualization.draw_geometries([meshLods[100]])

def generateLevelsOfDetails(mesh, lods: list, fileFormat: str, outputPath: str) -> dict:
    meshLods = {}
    for i in lods:
        # Export data to path
        currMeshLod = mesh.simplify_quadric_decimation(i)
        open3d.io.write_triangle_mesh(outputPath + "lod_" + str(i) + fileFormat, currMeshLod)
        
        meshLods[i] = currMeshLod
    
    print(f"Generated {i} LODS successful.")
    return meshLods

if __name__ == "__main__":
    main()
    
