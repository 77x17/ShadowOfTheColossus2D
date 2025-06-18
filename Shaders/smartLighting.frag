uniform sampler2D texture;
uniform float aspectRatio;

// Multiple light system
uniform int numLights;
uniform vec2 lightPositions[8];   // Tối đa 8 lights
uniform float lightRadii[8];      // Bán kính cho từng light
uniform vec3 lightColors[8];      // Màu sắc cho từng light  
uniform float lightIntensities[8]; // Cường độ cho từng light

uniform float darkness;           // Độ tối toàn cục

// Fog uniforms
uniform float fogDensity;
uniform float clearRadius;
uniform vec3 fogColor;

// Tint uniforms
uniform vec3 tintColor;
uniform float tintStrength;

void main() {
    vec2 uv = gl_TexCoord[0].xy;
    vec4 color = texture2D(texture, uv);
    
    // Điều chỉnh UV theo aspect ratio
    vec2 adjustedUV = uv;
    if (aspectRatio > 1.0) {
        adjustedUV.x *= aspectRatio;
    } else {
        adjustedUV.y /= aspectRatio;
    }
    
    // === 1. ÁP DỤNG TINT COLOR TRƯỚC ===
    if (tintStrength > 0.01) {
        color.rgb *= mix(vec3(1.0), tintColor, tintStrength);
    }
    
    // === 2. TÍNH TOÁN MULTIPLE LIGHT SOURCES ===
    float totalLightContribution = 0.0;
    vec3 totalLightColor = vec3(0.0);
    
    for (int i = 0; i < numLights && i < 8; ++i) {
        if (lightIntensities[i] <= 0.0) continue;
        
        // Điều chỉnh vị trí light theo aspect ratio
        vec2 adjustedLightPos = lightPositions[i];
        adjustedLightPos.y = 1.0 - adjustedLightPos.y; // Flip Y
        
        if (aspectRatio > 1.0) {
            adjustedLightPos.x *= aspectRatio;
        } else {
            adjustedLightPos.y /= aspectRatio;
        }
        
        // Tính khoảng cách
        float dist = distance(adjustedUV, adjustedLightPos);
        
        // Tính light falloff (smooth falloff)
        float lightFalloff = 1.0 - smoothstep(0.0, lightRadii[i], dist);
        lightFalloff = pow(lightFalloff, 2.0); // Quadratic falloff cho realistic hơn
        
        // Áp dụng intensity
        float lightStrength = lightFalloff * lightIntensities[i];
        
        // Cộng dồn light contribution
        totalLightContribution = max(totalLightContribution, lightStrength);
        
        // Blend màu sắc lights
        float weight = lightStrength / max(totalLightContribution, 0.001);
        totalLightColor = mix(totalLightColor, lightColors[i], weight);
    }
    
    // Clamp light contribution
    totalLightContribution = clamp(totalLightContribution, 0.0, 1.0);
    
    // === 3. ÁP DỤNG FOG (giữ nguyên logic từ NaturalEffects) ===
    if (fogDensity > 0.01) {
        // Tính fog dựa trên light gần nhất
        float minDistToLight = 1.0;
        for (int i = 0; i < numLights && i < 8; ++i) {
            if (lightIntensities[i] <= 0.0) continue;
            
            vec2 adjustedLightPos = lightPositions[i];
            adjustedLightPos.y = 1.0 - adjustedLightPos.y;
            
            if (aspectRatio > 1.0) {
                adjustedLightPos.x *= aspectRatio;
            } else {
                adjustedLightPos.y /= aspectRatio;
            }
            
            float dist = distance(adjustedUV, adjustedLightPos);
            minDistToLight = min(minDistToLight, dist);
        }
        
        float clearArea = 1.0 - smoothstep(0.0, clearRadius, minDistToLight);
        float finalFogDensity = mix(0.0, fogDensity, 1.0 - clearArea);
        
        // Giữ nguyên độ sáng
        float currentBrightness = (color.r + color.g + color.b) / 3.0;
        color.rgb = mix(color.rgb, fogColor, finalFogDensity);
        float newBrightness = (color.r + color.g + color.b) / 3.0;
        
        if (newBrightness > currentBrightness && currentBrightness > 0.0) {
            float ratio = currentBrightness / newBrightness;
            color.rgb *= ratio;
        }
    }
    
    // === 4. ÁP DỤNG DARKNESS VÀ LIGHTS ===
    float finalDarkness = darkness;
    
    // Áp dụng light contribution để giảm darkness
    float lightReduction = totalLightContribution;
    finalDarkness = finalDarkness * (1.0 - lightReduction);
    
    // Áp dụng màu sắc từ lights
    if (totalLightContribution > 0.01) {
        // Blend với màu sắc của lights
        color.rgb = mix(color.rgb, color.rgb * totalLightColor, totalLightContribution * 0.3);
    }
    
    // Áp dụng darkness cuối cùng
    color.rgb *= (1.0 - finalDarkness);
    
    gl_FragColor = color;
}