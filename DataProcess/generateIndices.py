# Credits https://stackoverflow.com/questions/14396788/how-can-i-generate-indices-from-vertex-list-in-linear-time

# Divisions of how many vertices per line scanned by the lidar, 
# this depends on how you configure it, how often it sends a ray.
# https://www.youtube.com/watch?v=FKLbihqDLsg

vertSet: dict = {}
indexList: list = []
normalsList: list = []

def vecSubtraction(pointA: dict, pointB: dict) -> dict:
    outputVec: dict = {'i': 0, 'j': 0, 'k': 0}
    
    outputVec['i'] = float(pointB['x']) - float(pointA['x'])
    outputVec['j'] = float(pointB['y']) - float(pointA['y'])
    outputVec['k'] = float(pointB['z']) - float(pointA['z'])
    
    for char in ['i', 'j', 'k']:
        if outputVec[char] == -0.0:
            outputVec[char] = 0.0
    
    return outputVec

def crossProduct(vecA: dict, vecB: dict) -> dict:
    outputVec: dict = {'i': 0, 'j': 0, 'k': 0}
    
    outputVec['i'] = vecA['j'] * vecB['k'] - vecA['k'] * vecB['j']
    outputVec['j'] = vecA['k'] * vecB['i'] - vecA['i'] * vecB['k']
    outputVec['k'] = vecA['i'] * vecB['j'] - vecA['j'] * vecB['i']
    
    for char in ['i', 'j', 'k']:
        if outputVec[char] == -0.0:
            outputVec[char] = 0.0
    
    return outputVec

def appendNormal(v1: int, v2: int, v3: int):
    vecA: dict = vecSubtraction(vertSet[v1], vertSet[v2])
    vecB: dict = vecSubtraction(vertSet[v1], vertSet[v3])
    currNormal: dict = crossProduct(vecA, vecB)
    normalsList.append(currNormal)

def main():
    # Given a list of vertices derived from a lidar scanner, find a way to generate an integer list of 
    # indices from the set of vertex data.
    
    # Simplest method is to use a set data structure
    # Pairing a vertex to an index integer
    index: int = 0
    
    # Establish connection to files
    inputPath: str = ""
    inputName: str = "test3x3.txt"
    DIV = 2
    f = open(inputPath + inputName, "r")

    # Loop through file and seperate the vertices, adding them to the dictionary
    for line in f:
        # Split each float in each line and append to our current vertex
        line = line.replace("\n", "")
        vertices: list = line.split(" ")
        
        currVert: dict = {'x': vertices[0], 'y': vertices[1], 'z': vertices[2]}
        vertSet[index] = currVert
        index += 1
    
    # Loop through the vertices, create indicies
    # Getting indices for top triangle, it's (x+1, y+div+1)->(x+1, y)->->(x, y+div+1)
    # Getting indices for top triangle, it's (x+1, y)->(x, y)->(x, y+div+1)
    for currIndex in vertSet.keys():
        # v_3 v_4
        # v_1 v_2
        v1: int = currIndex
        v2: int = currIndex + 1
        v3: int = currIndex + DIV + 1
        v4: int = currIndex + DIV + 2
        
        # Check if right vert exists, and upper vert exists
        if currIndex % (DIV + 1) < DIV:
            if (currIndex + DIV + 1) % (DIV + 1) < DIV:
                # Cut off top level
                if currIndex < ((index) / (DIV + 1)) * DIV:
                    # Append the indicies for the top and bottom triangle
                    indexList.append(v4)
                    indexList.append(v2)
                    indexList.append(v3)
                    indexList.append(v2)
                    indexList.append(v1)
                    indexList.append(v3)
                    
                    # Append normals for current point (should be everything except last column, first row)
                    appendNormal(v1, v2, v3)
                else: 
                    # First row, not last column
                    v3 = currIndex
                    v4 = currIndex + 1
                    v2 = currIndex - DIV
                    appendNormal(v2, v4, v3)
        else:
            # Last column
            # Check if not first row, last column
            if currIndex != DIV:
                v4 = currIndex
                v3 = currIndex - 1
                v2 = currIndex - DIV - 1
                appendNormal(v4, v3, v2)
            else:
                v2 = currIndex
                v3 = currIndex + DIV
                v4 = currIndex + DIV + 1
                appendNormal(v2, v3, v4)
    
    # Output both lists
    print("Vertices: ")
    print(vertSet)
    print("Indices ")
    print(indexList)
    print("Normals: ")
    print(normalsList)

if __name__ == "__main__":
    main()
