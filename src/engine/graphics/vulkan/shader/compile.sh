glslc shader.vert -o vert.spv
glslc shader.frag -o frag.spv
bin2header vert.spv
bin2header frag.spv