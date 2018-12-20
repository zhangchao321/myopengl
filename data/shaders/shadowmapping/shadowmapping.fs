#version 330 core

out vec4 outColor;

in vec3 v_fragPos;
in vec3 v_normal;
in vec2 v_texCoords;
in vec4 v_fragPosLightSpace;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;
uniform vec3 lightPos;
uniform vec3 viewPos;
 
float ShadowCalculation(vec4 fragPosLightSpace)
{ 
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w; //深度除法，获取在投影空间的坐标
    projCoords = projCoords * 0.5 + 0.5; //将位置数据归一化到0-1之间 
    float currentDepth = projCoords.z; //获取当前位置的当前深度
    vec3 normal = normalize(v_normal);
    vec3 lightDir = normalize(lightPos - v_fragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005); //计算阴影偏移系数
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);//纹理坐标和纹理大小的映射系数，一个像素所对应的纹理坐标长度为texelSize
	//对当前像素所对应的9个最近深度采样
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += (currentDepth - bias) > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    if(projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}

void main()
{           
    vec3 color = texture(diffuseTexture, v_texCoords).rgb;
    vec3 normal = normalize(v_normal);
    vec3 lightColor = vec3(0.9); 

    vec3 ambient = 0.2 * color;

    vec3 lightDir = normalize(lightPos - v_fragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - v_fragPos);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor; 

    float shadow =ShadowCalculation(v_fragPosLightSpace);                      
    shadow = min(shadow, 0.75); 
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
    outColor = vec4(lighting,1.0f);
}