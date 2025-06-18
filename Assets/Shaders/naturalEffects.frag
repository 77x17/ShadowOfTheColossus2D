uniform sampler2D texture;
uniform vec2 lightPosition; // relative [0,1]
uniform float aspectRatio;  // tỷ lệ khung hình (width/height)

// Darkness uniforms
uniform float darkness;     // 0 = sáng hoàn toàn, 1 = tối đen
uniform float lightRadius;  // bán kính ánh sáng (0.0 - 1.0)

// Fog uniforms
uniform float fogDensity;   // 0 = không có sương mù, 1 = sương mù dày đặc
uniform float clearRadius;  // bán kính vùng không có sương mù (0.0 - 1.0)
uniform vec3 fogColor;      // màu sắc sương mù

// Tint uniforms (mới thêm)
uniform vec3 tintColor;     // màu tint (1,1,1 = không tint)
uniform float tintStrength; // độ mạnh tint (0 = không có, 1 = tint hoàn toàn)

void main() {
    vec2 uv = gl_TexCoord[0].xy;
    vec4 color = texture2D(texture, uv);

    // Tính toán vị trí và khoảng cách
    vec2 correctedPos = vec2(lightPosition.x, 1.0 - lightPosition.y);
    vec2 adjustedUV = uv;
    vec2 adjustedPos = correctedPos;
    
    if (aspectRatio > 1.0) {
        adjustedUV.x  *= aspectRatio;
        adjustedPos.x *= aspectRatio;
    } else {
        adjustedUV.y  /= aspectRatio;
        adjustedPos.y /= aspectRatio;
    }
    
    float dist = distance(adjustedUV, adjustedPos);
    
    // === 1. ÁP DỤNG TINT COLOR TRƯỚC (hiệu ứng màu buổi chiều) ===
    if (tintStrength > 0.01) {
        color.rgb *= mix(vec3(1.0), tintColor, tintStrength);
    }
    
    // === 2. ÁP DỤNG FOG (nhưng giữ nguyên độ tối) ===
    if (fogDensity > 0.01) {
        float clearArea = 1.0 - smoothstep(0.0, clearRadius, dist);
        float finalFogDensity = mix(0.0, fogDensity, 1.0 - clearArea);
        
        // Tính độ sáng hiện tại để giữ nguyên sau khi áp fog
        float currentBrightness = (color.r + color.g + color.b) / 3.0;
        
        // Áp dụng fog
        color.rgb = mix(color.rgb, fogColor, finalFogDensity);
        
        // Tính độ sáng sau fog
        float newBrightness = (color.r + color.g + color.b) / 3.0;
        
        // Điều chỉnh để giữ nguyên độ tối (tránh fog làm sáng lên)
        if (newBrightness > currentBrightness && currentBrightness > 0.0) {
            float ratio = currentBrightness / newBrightness;
            color.rgb *= ratio;
        }
    }
    
    // === 3. ÁP DỤNG DARKNESS CUỐI CÙNG ===
    if (darkness > 0.01) {
        float light = 1.0 - smoothstep(0.0, lightRadius, dist);
        float darknessFactor = mix(darkness, 0.0, light);
        color.rgb *= (1.0 - darknessFactor);
    }

    gl_FragColor = color;
}
