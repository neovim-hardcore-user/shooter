import pygame
from PIL import Image

def load_obj(file_path):
    vertices = []
    normals = []

    material_ordered_faces = {}
    uvs = []

    material = ""

    with open(file_path, "r") as file:
        for line in file:
            if line.startswith("usemtl "):
                material = line.split()[1]
                material_ordered_faces[material] = []
            if line.startswith("v "):
                vertex = list(map(float, line.split()[1:]))
                vertices.append(vertex)

            if line.startswith("vn "):
                normal = list(map(float, line.split()[1:]))
                normals.append(normal)

            if line.startswith("vt "):
                uv = list(map(float, line.split()[1:]))
                uvs.append(uv)


            if line.startswith("f "):
                face = line.split()[1:]

                face = [f.split("/") for f in face]
                face = [[int(i) - 1 for i in f] for f in face]

                material_ordered_faces[material].append(face)

    return vertices, normals, uvs, material_ordered_faces


def load_image(file_path):
    image = Image.open(file_path)
    width, height = image.size

    pixels = list(image.getdata())
    pixels = [pixels[i * width:(i + 1) * width] for i in range(height)]
    

    return pixels, width, height

def RGB(r, g, b):
     return ((((int(r) >> 3) & 0b11111) | ((int(g) << 2) & 0b1111100000) | ((int(b) << 6) & 0b11110000000000) | ((255 << 8) & 0xc000)) & 0x3fff)


vertices, normals, uvs, faces = load_obj(input("model to convert: "));



FIXED_POINT = 2**16

model_file = '#include "matrix_vector.h"\n\n'
model_file += "int triangle_count = " + str(len(faces["player"])) + ";\n"
model_file += "int vertex_count = " + str(len(vertices)) + ";\n\n"


model_file += "vec4 transformed_vertices[" + str(len(vertices)) + "];\n\n"
model_file += "const vec3 vertices[" + str(len(vertices)) + "] = {\n"

i = 1
for vertex in vertices:
    model_file += "{" + str(vertex[0]) + ", " + str(vertex[1]) + ", " + str(vertex[2]) + "}, "
    
    if i == 3:
        model_file += "\n"
        i = 0
    i += 1

model_file += "}; \n\n"

model_file += "const short colors[" + str(len(faces["player"])) + "] = {\n"

b = 1
for i in range(len(faces["player"])):
    normal = normals[faces["player"][i][0][2]]
    model_file += str(RGB((-normal[0] * 0.5 + 0.5) * 255, (-normal[1] * 0.5 + 0.5) * 255, (-normal[2] * 0.5 + 0.5) * 255)) + ", "
    
    if b == 10:
        model_file += "\n"
        b = 0
    b += 1

model_file += "}; \n\n"

"""model_file += "const uint16_t face_colors[" + str(len(faces["player"])) + "] = {\n";

for face in faces["player"]:
    model_file += str(face[0][2]) + ",\n" 
    
model_file += "}; \n\n"
"""

model_file += "const uint16_t faces[" + str(len(faces["player"])) + "][3] = {\n";

i = 1
for face in faces["player"]:
    model_file += "{" + str(face[0][0]) + ", " + str(face[1][0]) + ", " + str(face[2][0]) + "}, " 

    if i == 5:
        model_file += "\n"
        i = 0
    i += 1

model_file += "}; \n\n"




"""model_file += "uint16_t faces[" + str(len(faces["player"])) + "][3][3] = {\n";

for face in (faces["player"]):
    model_file += "{"
    model_file += "{" + str(face[0][0]) + ", " + str(face[0][1]) + ", " + str(face[0][2]) + "},"
    model_file += "{" + str(face[1][0]) + ", " + str(face[1][1]) + ", " + str(face[1][2]) + "},"
    model_file += "{" + str(face[2][0]) + ", " + str(face[2][1]) + ", " + str(face[2][2]) + "}"
    model_file += "}, \n"
model_file += "}; \n\n"
"""

with open(input("name of the header: "), "w") as f:
    f.write(model_file)
