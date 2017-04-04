// Tessellation domain shader
// After tessellation the domain shader processes the all the vertices
// S:\git\uniShaders\Shaders\E03_SpecularLighting\shaders\light_vs.hlsl(11,33-35): error X3530: invalid register specification, expected 'b' binding when cb used
cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer TessellationWarpBuffer : register(b2)
{
    int powers;
    float repeats;
    float severity;
    float lerpAmount;
    float3 baseColour;
    bool targetSin;
}

struct ConstantOutputType
{
    float edges[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;
};

struct InputType
{
    float3 position : POSITION;
    float4 colour : COLOR;
};

struct OutputType
{
    float4 position : SV_POSITION;
    float4 colour : COLOR;
};

// JAVA REFERENCE IMPLEMENTATION OF IMPROVED NOISE - COPYRIGHT 2002 KEN PERLIN.
//class ImprovedNoise
//{
    //, permutation[512]
 //   int q[512];
   //  =
   // {
   //     151, 160, 137, 91, 90, 15,
   //131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
   //190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
   //88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
   //77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
   //102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
   //135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
   //5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
   //223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
   //129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
   //251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
   //49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
   //138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180,
   //     151, 160, 137, 91, 90, 15,
   //131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
   //190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
   //88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
   //77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
   //102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
   //135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
   //5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
   //223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
   //129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
   //251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
   //49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
   //138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
   // };

   // void init(int pe[512])
   // {
   ////     //for (int i = 0; i < 256; i++)
   ////     //{
   ////     //    p[256 + i] = p[i] = permutation[i];
   ////     //}

   ////     int pe[512] = 
   ////     {
   ////         151, 160, 137, 91, 90, 15,
   ////131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
   ////190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
   ////88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
   ////77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
   ////102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
   ////135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
   ////5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
   ////223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
   ////129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
   ////251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
   ////49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
   ////138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180,
   ////     151, 160, 137, 91, 90, 15,
   ////131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
   ////190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
   ////88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
   ////77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
   ////102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
   ////135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
   ////5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
   ////223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
   ////129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
   ////251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
   ////49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
   ////138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
   //// };
   //     for (int i = 0; i < 512; i++)
   //     {
   //         q[i] = pe[i];
   //     }

   // }

    double fade(double t)
    {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    double myLerp(double t, double a, double b)
    {
        return a + t * (b - a);
    }

    static double grad(int hash, double x, double y, double z)
    {
        int h = hash & 15; // CONVERT LO 4 BITS OF HASH CODE
        double u = h < 8 ? x : y, // INTO 12 GRADIENT DIRECTIONS.
                 v = h < 4 ? y : h == 12 || h == 14 ? x : z;
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }

    double noise(double x, double y, double z)
    {
    int q[512]
     =
    {
        151, 160, 137, 91, 90, 15,
   131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
   190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
   88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
   77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
   102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
   135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
   5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
   223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
   129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
   251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
   49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
   138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180,
        151, 160, 137, 91, 90, 15,
   131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
   190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
   88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
   77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
   102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
   135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
   5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
   223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
   129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
   251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
   49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
   138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
    };

        int X = (int) floor(x) & 255, // FIND UNIT CUBE THAT
            Y = (int) floor(y) & 255, // CONTAINS POINT.
            Z = (int) floor(z) & 255;

        x -= floor(x); // FIND RELATIVE X,Y,Z
        y -= floor(y); // OF POINT IN CUBE.
        z -= floor(z);

        double u = fade(x), // COMPUTE FADE CURVES
                v = fade(y), // FOR EACH OF X,Y,Z.
                w = fade(z);

        int A = q[X] + Y, AA = q[A] + Z, AB = q[A + 1] + Z, // HASH COORDINATES OF
            B = q[X + 1] + Y, BA = q[B] + Z, BB = q[B + 1] + Z; // THE 8 CUBE CORNERS,

        return myLerp(w, myLerp(v, myLerp(u, grad(q[AA], x, y, z), // AND ADD
                                        grad(q[BA], x - 1, y, z)), // BLENDED
                                myLerp(u, grad(q[AB], x, y - 1, z), // RESULTS
                                        grad(q[BB], x - 1, y - 1, z))), // FROM  8
                        myLerp(v, myLerp(u, grad(q[AA + 1], x, y, z - 1), // CORNERS
                                        grad(q[BA + 1], x - 1, y, z - 1)), // OF CUBE
                                myLerp(u, grad(q[AB + 1], x, y - 1, z - 1),
                                        grad(q[BB + 1], x - 1, y - 1, z - 1))));
    }

//float noise(float x, float y, float z)
//{
//    return 2;
//}

//};






[domain("tri")]
OutputType main(ConstantOutputType input, float3 uvwCoord : SV_DomainLocation, const OutputPatch<InputType, 4> patch)
{
    OutputType output;
    //ImprovedNoise perlinNoise;
    float3 vertexPosition;

   // int p[512]
   //  =
   // {
   //     151, 160, 137, 91, 90, 15,
   //131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
   //190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
   //88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
   //77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
   //102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
   //135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
   //5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
   //223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
   //129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
   //251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
   //49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
   //138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180,
   //     151, 160, 137, 91, 90, 15,
   //131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
   //190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
   //88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
   //77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
   //102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
   //135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
   //5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
   //223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
   //129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
   //251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
   //49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
   //138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
   // };

   // init(p);
     
    // Determine the position of the new vertex.
	// Invert the y and Z components of uvwCoord as these coords are generated in UV space and therefore y is positive downward.
	// Alternatively you can set the output topology of the hull shader to cw instead of ccw (or vice versa).
    vertexPosition = uvwCoord.x * patch[0].position + uvwCoord.y * patch[1].position + uvwCoord.z * patch[2].position;
    
    float4 sinColourModifier = float4(baseColour.x, baseColour.y, baseColour.z, 1.0f);
    float4 cosColourModifier = sinColourModifier;
    float4 colourModifier = sinColourModifier;
    float3 radialVector = vertexPosition;
    float3 sinVertexPosition = vertexPosition;
    float3 cosVertexPosition = vertexPosition;
    float radius = length(radialVector);

 //   float vxr = vertexPosition.x * repeats;
 //   float vyr = vertexPosition.y * repeats;
 //   float vzr = vertexPosition.z * repeats;
    
	////// Calculate sin wave surface modifier according to the parametric equations:
 //   // x1 = x0 + severity*x0*(sin(repeats*x0)*sin(repeats*y0)*sin(repeats*z0))^powers
 //   // y1 = y0 + severity*y0*(sin(repeats*x0)*sin(repeats*y0)*sin(repeats*z0))^powers
 //   // z1 = z0 + severity*z0*(sin(repeats*x0)*sin(repeats*y0)*sin(repeats*z0))^powers

 //   float sinWarp = 1.0f;
 //   float sinvx = sin(vxr);
 //   float sinvy = sin(vyr);
 //   float sinvz = sin(vzr);
 //   float sinvxyz = sinvx * sinvy * sinvz;

 //   for (int i = 0; i < powers; i++)
 //   {
 //       sinWarp *= sinvxyz;
 //   }

 //   float sinSev = sinWarp * severity;

 //   sinVertexPosition.x += radialVector.x * sinSev;
 //   sinVertexPosition.y += radialVector.y * sinSev;
 //   sinVertexPosition.z += radialVector.z * sinSev;
   
 //   //// Calculate cos wave surface modifier according to the parametric equations:
 //   // x1 = x0 + severity*x0*(cos(repeats*x0)*cos(repeats*y0)*cos(repeats*z0))^powers
 //   // y1 = y0 + severity*y0*(cos(repeats*x0)*cos(repeats*y0)*cos(repeats*z0))^powers
 //   // z1 = z0 + severity*z0*(cos(repeats*x0)*cos(repeats*y0)*cos(repeats*z0))^powers

 //   float cosWarp = 1.0f;
 //   float cosvx = cos(vxr);
 //   float cosvy = cos(vyr);
 //   float cosvz = cos(vzr);
 //   float cosvxyz = cosvx * cosvy * cosvz;
    
 //   for (int j = 0; j < powers; j++)
 //   {
 //       cosWarp *= cosvxyz;
 //   }

 //   float cosSev = cosWarp * severity;

 //   cosVertexPosition.x += radialVector.x * cosSev;
 //   cosVertexPosition.y += radialVector.y * cosSev;
 //   cosVertexPosition.z += radialVector.z * cosSev;

	//// Lerp between cos function and sin function
 //   if (targetSin)
 //   {
 //       vertexPosition.x = lerp(cosVertexPosition.x, sinVertexPosition.x, lerpAmount);
 //       vertexPosition.y = lerp(cosVertexPosition.y, sinVertexPosition.y, lerpAmount);
 //       vertexPosition.z = lerp(cosVertexPosition.z, sinVertexPosition.z, lerpAmount);
 //   }
 //   else
 //   {
 //       vertexPosition.x = lerp(sinVertexPosition.x, cosVertexPosition.x, 1 - lerpAmount);
 //       vertexPosition.y = lerp(sinVertexPosition.y, cosVertexPosition.y, 1 - lerpAmount);
 //       vertexPosition.z = lerp(sinVertexPosition.z, cosVertexPosition.z, 1 - lerpAmount);
 //   }

    vertexPosition += vertexPosition + (noise(vertexPosition.x, vertexPosition.y, vertexPosition.z) * vertexPosition);
    vertexPosition += vertexPosition + (noise(vertexPosition.x, vertexPosition.y, vertexPosition.z) * vertexPosition);

    // Calculate the position of the new vertex (in 3D space)
    output.position = float4(vertexPosition, 1.0f);
  
    // Get tone of inverse base colour based on distance from centre of the sphere
    float tone = radius / (length(vertexPosition) * 2.0f);

    // Send the input color into the pixel shader.
    colourModifier.x = 1 - (baseColour.x + saturate(tone));
    colourModifier.y = 1 - (baseColour.y + saturate(tone));
    colourModifier.z = 1 - (baseColour.z + saturate(tone));
    colourModifier.w = 1.0f;
    
    colourModifier = saturate(colourModifier);

    output.colour = colourModifier;

    return output;
}

