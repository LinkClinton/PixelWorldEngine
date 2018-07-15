import sys

def CreateCharArray(fileName, suffixName):
    shaderFile = open(fileName + suffixName, "r")
    
    data = shaderFile.readlines()
    
    shaderFile.close()

    result = "constexpr char "+ fileName + "DefaultShaderCode[] = \""

    for item in data:
        item = item.replace('\n', ' ')
        result += item + r'\n'
    pass

    result+="\";"

    return result

result = ""

result += "#include \"pch.hpp\"\n\n"

result += "namespace PixelWorldEngine {\n"

result += "\t" + CreateCharArray("vsPixelWorld", ".hlsl") + "\n"
result += "\t" + CreateCharArray("psPixelWorld", ".hlsl") + "\n"
result += "\t" + CreateCharArray("vsApplication", ".hlsl") + "\n"
result += "\t" + CreateCharArray("psApplication", ".hlsl") + "\n"
result += "\t" + CreateCharArray("lowVsPixelWorld", ".hlsl") + "\n"
result += "\t" + CreateCharArray("lowPsPixelWorld", ".hlsl") + "\n"

result += "\n}"

hppFile = open("EngineDefaultResource.hpp", "w")

hppFile.write(result)

hppFile.close()



