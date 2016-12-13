// Tessellation domain shader
// After tessellation the domain shader processes the all the vertices
// S:\git\uniShaders\Shaders\E03_SpecularLighting\shaders\light_vs.hlsl(11,33-35): error X3530: invalid register specification, expected 'b' binding when cb used
cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix lightView;
    matrix lightProjection;
};

cbuffer TessellationWarpBuffer : register(b2)
{
	int powers;
	float repeats;
	float severity;
	float lerpAmount;
	float3 baseColour;
	bool targetSin;
    float3 lightPosition;
    float padding;
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
	float3 viewDirection : TEXCOORD1;
};

struct OutputType
{
    float4 position : SV_POSITION;
    float4 colour : COLOR;
	float3 viewDirection : TEXCOORD1;
	float3 normal : NORMAL;
    float3 position3D : TEXCOORD2;
    float3 lightPos : TEXCOORD3;
    float4 lightViewPosition : TEXCOORD4;
};

[domain("tri")]
OutputType main(ConstantOutputType input, float3 uvwCoord : SV_DomainLocation, const OutputPatch<InputType, 4> patch)
{
	float3 vertexPosition, normals;
	float3 sinVertexPosition, sinNormal;
	float3 cosVertexPosition, cosNormal;
    OutputType output;
 
    // Determine the position of the new vertex.
	// Invert the y and Z components of uvwCoord as these coords are generated in UV space and therefore y is positive downward.
	// Alternatively you can set the output topology of the hull shader to cw instead of ccw (or vice versa).
	vertexPosition = uvwCoord.x * patch[0].position + uvwCoord.y * patch[1].position + uvwCoord.z * patch[2].position;
    
	float4 sinColourModifier = float4(baseColour.x, baseColour.y, baseColour.z, 1.0f);
	float4 cosColourModifier = sinColourModifier;
	float4 colourModifier = sinColourModifier;
	float3 radialVector = vertexPosition;
	sinVertexPosition = vertexPosition;
	cosVertexPosition = vertexPosition;

	float vxr = vertexPosition.x * repeats;
	float vyr = vertexPosition.y * repeats;
	float vzr = vertexPosition.z * repeats;


	//// Calculate sin wave surface modifier
	float sinWarp = 1.0f;
	float sinvx = sin(vxr);
	float sinvy = sin(vyr);
	float sinvz = sin(vzr);
	float sinvxyz = sinvx * sinvy * sinvz;

	for (int i = 0; i < powers; i++)
	{
		sinWarp *= sinvxyz;
	}

	float sinSev = sinWarp * severity;

	sinVertexPosition.x += radialVector.x * sinSev;
	sinVertexPosition.y += radialVector.y * sinSev;
	sinVertexPosition.z += radialVector.z * sinSev;

	// INCORRECT // normal = -1 / ( s (sin(r x) sin(r y) sin(r z))^p + p r s x cot(r x) (sin(r x) sin(r y) sin(r z))^p + 1)
    
    float radius = length(radialVector);
    float u = atan(vertexPosition.y / vertexPosition.x);
    float v = acos(vertexPosition.z / radius);
    float su = sin(u);
    float cu = cos(u);
    float sv = sin(v);
    float cv = cos(v);
    
    // Parametric equation of the shape

    // Intermediates
    float c = radius;
    float a = severity * radius;
    float b = repeats * radius;
    float p = powers;
    float sinX = sin(b*cu*sv);
    float sinY = sin(b*su*sv);
    float sinZ = sin(b*cv);
    float cosX = cos(b*cu*sv);
    float cosY = cos(b*su*sv);
    float cosZ = cos(b*cv);
    float tanX = tan(b * cu * sv);
    float tanY = tan(b * su * sv);
    float tanZ = tan(b * cv);
    float cotX = 1 / tanX;
    float cotY = 1 / tanY;
    float cotZ = 1 / tanZ;
    float sinCalc = sinX*sinY*sinZ;
    float cosCalc = cosX*cosY*cosZ;
    float sinPowers = 1.0f;
    float cosPowers = 1.0f;
    float sinPowersLess = 1.0f;
    float cosPowersLess = 1.0f;

     for (int pow = 0; pow < p; pow++)
    {
        if (pow != 0)
        {
            sinPowersLess *= sinCalc;
            cosPowersLess *= cosCalc;
        }
       sinPowers *= sinCalc;
       cosPowers *= cosCalc;
    }

    float dvSinDiffInter = (b*su*cv*sinZ*cosY*sinX - b*sv*cosZ*sinY*sinX + b*cu*cv*sinY*sinZ*cosX);

    

    /////////////////////////////////
    //        SIN CONFIG           //
    /////////////////////////////////
    /////////////////////////////////
    // Equation of x
    // x = (c * cos(u) * sin(v)) + (a * cos(u) * sin(v))*((sin(b * cos(u) * sin(v)) * sin(b * sin(u) * sin(v)) * sin(b * cos(v)))^p)
    // with intermediates
    //sinVertexPosition.x = (c * cu * sv) + ((a * cu * sv) * sinPowers);
    
    //d)/(du)(x = c cos(u) sin(v)) = -c sin(u) sin(v)
    //(d)/(du)(x = a cos(u) sin(v) (sin(b cos(u) sin(v)) sin(b sin(u) sin(v)) sin(b cos(v)))^p) = -a sin(v) (sin(b sin(u) sin(v)) sin(b cos(v)) sin(b cos(u) sin(v)))^p (-b p cos^2(u) sin(v) cot(b sin(u) sin(v))+b p sin(u) cos(u) sin(v) cot(b cos(u) sin(v))+sin(u))
    
        // differentiated with respect to u
        // dx/du = -(c * sin(u) * sin(v)) - (a * sin(v) * (sin(b * sin(u) * sin(v)) * sin(b * cos(v)) * sin(b * cos(u) * sin(v)))^p * (-b * p * cos^2(u) * sin(v) * cot(b * sin(u) * sin(v)) + b * p * sin(u) * cos(u) * sin(v) * cot(b * cos(u) * sin(v))+sin(u)))
        // with intermediates
        float dxdu = (-1*c*su*sv) - (a*sv*sinPowers*-1*b*p*cu*cu*sv*cotY) + (b*p*su*cu*sv*cotX) + su;

    // (d)/(dv)(x = c cos(u) sin(v)) = c cos(u) cos(v)
    // (d)/(dv)(x = a cos(u) sin(v) (sin(b cos(u) sin(v)) sin(b sin(u) sin(v)) sin(b cos(v)))^p) = (0 = a p cos(u) sin(v) (b sin(u) cos(v) sin(b cos(v)) cos(b sin(u) sin(v)) sin(b cos(u) sin(v)) - b sin(v) cos(b cos(v)) sin(b sin(u) sin(v)) sin(b cos(u) sin(v)) + b cos(u) cos(v) sin(b sin(u) sin(v)) sin(b cos(v)) cos(b cos(u) sin(v))) (sin(b sin(u) sin(v)) sin(b cos(v)) sin(b cos(u) sin(v)))^(p - 1) + a cos(u) cos(v) (sin(b sin(u) sin(v)) sin(b cos(v)) sin(b cos(u) sin(v)))^p)

        // differentiated with respect to v
        // dx/dv = (c*cos(u)*cos(v)) + (a p cos(u) sin(v) (b sin(u) cos(v) sin(b cos(v)) cos(b sin(u) sin(v)) sin(b cos(u) sin(v)) - b sin(v) cos(b cos(v)) sin(b sin(u) sin(v)) sin(b cos(u) sin(v)) + b cos(u) cos(v) sin(b sin(u) sin(v)) sin(b cos(v)) cos(b cos(u) sin(v))) (sin(b sin(u) sin(v)) sin(b cos(v)) sin(b cos(u) sin(v)))^(p - 1) + a cos(u) cos(v) (sin(b sin(u) sin(v)) sin(b cos(v)) sin(b cos(u) sin(v)))^p)
	    // with intermediates
        float dxdv = (c*cu*cv) + ((a*p*cu*sv*dvSinDiffInter*sinPowersLess) + (a*cu*cv*sinPowers));

    /////////////////////////////////
    // Equation of y
    // y = (c * sin(u) * sin(v)) + (a * sin(u) * sin(v))*((sin(b * cos(u) * sin(v)) * sin(b * sin(u) * sin(v)) * sin(b * cos(v)))^p)
    // with intermediates
   // sinVertexPosition.y = (c * su * sv) + (a * su * sv * sinPowers);

    //(d)/(du)(y = c sin(u) sin(v)) = c cos(u) sin(v)
    //(d)/(du)(y = a sin(u) sin(v) (sin(b cos(u) sin(v)) sin(b sin(u) sin(v)) sin(b cos(v)))^p) = a sin(v) (sin(b sin(u) sin(v)) sin(b cos(v)) sin(b cos(u) sin(v)))^p (-b p sin^2(u) sin(v) cot(b cos(u) sin(v)) + b p sin(u) cos(u) sin(v) cot(b sin(u) sin(v)) + cos(u))

        // differentiated with respect to u
        float dydu = (c*cu*sv) + ((a*sv*sinPowers*-1*b*p*su*su*sv*cotX) + (b*p*su*cu*sv*cotY) + cu);

    //(d)/(dv)(y = c sin(u) sin(v)) = c sin(u) cos(v)
    //(d)/(dv)(y = a sin(u) sin(v) (sin(b cos(u) sin(v)) sin(b sin(u) sin(v)) sin(b cos(v)))^p) = (0 = a p sin(u) sin(v) (b sin(u) cos(v) sin(b cos(v)) cos(b sin(u) sin(v)) sin(b cos(u) sin(v)) - b sin(v) cos(b cos(v)) sin(b sin(u) sin(v)) sin(b cos(u) sin(v)) + b cos(u) cos(v) sin(b sin(u) sin(v)) sin(b cos(v)) cos(b cos(u) sin(v))) (sin(b sin(u) sin(v)) sin(b cos(v)) sin(b cos(u) sin(v)))^(p - 1) + a sin(u) cos(v) (sin(b sin(u) sin(v)) sin(b cos(v)) sin(b cos(u) sin(v)))^p)

        // differentiated with respect to v
        float dydv = (c*su*cv) + (a*p*su*sv*dvSinDiffInter*sinPowersLess) + (a*su*cv*sinPowers);

    /////////////////////////////////
    // Equation of z
    // z = (c * cos(v)) + (a * cos(v))*((sin(b * cos(u) * sin(v)) * sin(b * sin(u) * sin(v)) * sin(b * cos(v)))^p)
    // with intermediates
   // sinVertexPosition.z = (c * cv) + (a * cv) * sinPowers;

    //(d)/(du)(z = c cos(v)) = 0
    //(d)/(du)(z = a cos(v) (sin(b cos(u) sin(v)) sin(b sin(u) sin(v)) sin(b cos(v)))^p) = a b p sin(v) cos(v) (sin(b sin(u) sin(v)) sin(b cos(v)) sin(b cos(u) sin(v)))^p (cos(u) cot(b sin(u) sin(v)) - sin(u) cot(b cos(u) sin(v)))

        // differentiated with respect to u
        float dzdu = a*b*p*sv*cv*sinPowers * (cu*cotY - su*cotX);

    //(d)/(dv)(z = c cos(v)) = -c sin(v)
    //(d)/(dv)(z = a cos(v) (sin(b cos(u) sin(v)) sin(b sin(u) sin(v)) sin(b cos(v)))^p) = (0 = a p cos(v) (b sin(u) cos(v) sin(b cos(v)) cos(b sin(u) sin(v)) sin(b cos(u) sin(v)) - b sin(v) cos(b cos(v)) sin(b sin(u) sin(v)) sin(b cos(u) sin(v)) + b cos(u) cos(v) sin(b sin(u) sin(v)) sin(b cos(v)) cos(b cos(u) sin(v))) (sin(b sin(u) sin(v)) sin(b cos(v)) sin(b cos(u) sin(v)))^(p - 1) - a sin(v) (sin(b sin(u) sin(v)) sin(b cos(v)) sin(b cos(u) sin(v)))^p)

        // differentiated with respect to v
        float dzdv = (-1*c*sv) + (a*p*cv*dvSinDiffInter*sinPowersLess) - (a*sv*sinPowers);

    /////////////////////////////////
    // Cross product of (dxdu, dydu, dzdu) X (dxdv, dydv, dzdv) = normal vector (will need normalizing)

    float3 sinNorms = cross(float3(dxdu, dydu, dzdu), float3(dxdv, dydv, dzdv));
    sinNorms = normalize(sinNorms);

    /////////////////////////////////
    //        COS CONFIG           //
    /////////////////////////////////
    /////////////////////////////////
    /////////////////////////////////
    // Equation of x
    // x = (c * cos(u) * sin(v)) + (a * cos(u) * sin(v))*((cos(b * cos(u) * sin(v)) * cos(b * sin(u) * sin(v)) * cos(b * cos(v)))^p)
    // with intermediates
   // cosVertexPosition.x = (c * cu * sv) + (a * cu * sv * cosPowers);
    
    // (d)/(du)(x = c cos(u) sin(v)) = -c sin(u) sin(v)
    // (d)/(du)(x = a cos(u) sin(v) (cos(b cos(u) sin(v)) cos(b sin(u) sin(v)) cos(b cos(v)))^p) = (0 = a p cos(u) sin(v) (b sin(u) sin(v) cos(b cos(v)) cos(b sin(u) sin(v)) sin(b cos(u) sin(v)) - b cos(u) sin(v) cos(b cos(v)) sin(b sin(u) sin(v)) cos(b cos(u) sin(v))) (cos(b cos(v)) cos(b cos(u) sin(v)) cos(b sin(u) sin(v)))^(p - 1) - a sin(u) sin(v) (cos(b cos(v)) cos(b cos(u) sin(v)) cos(b sin(u) sin(v)))^p)

        // differentiated with respect to u
        dxdu = (-1*c*su*sv) + (a*p*cu*sv*cosPowersLess*((b*su*sv*cosZ*cosY*sinX) - (b*cu*sv*cosZ*sinY*cosX))) - (a*su*sv*cosPowers);

    // (d)/(dv)(x = c cos(u) sin(v)) = c cos(u) cos(v)
    // (d)/(dv)(x = a cos(u) sin(v) (cos(b cos(u) sin(v)) cos(b sin(u) sin(v)) cos(b cos(v)))^p) = (0 = a p cos(u) sin(v) (b sin(v) sin(b cos(v)) cos(b cos(u) sin(v)) cos(b sin(u) sin(v)) - b cos(u) cos(v) cos(b cos(v)) cos(b sin(u) sin(v)) sin(b cos(u) sin(v)) - b sin(u) cos(v) cos(b cos(v)) sin(b sin(u) sin(v)) cos(b cos(u) sin(v))) (cos(b cos(v)) cos(b cos(u) sin(v)) cos(b sin(u) sin(v)))^(p - 1) + a cos(u) cos(v) (cos(b cos(v)) cos(b cos(u) sin(v)) cos(b sin(u) sin(v)))^p)
    
        // differentiated with respect to v
        dxdv = (c*cu*cv) + (a*p*cu*sv*cosPowersLess*((b*sv*sinZ*cosX*cosY) - (b*cu*cv*cosZ*cosY*sinX) - (b*su*cv*cosZ*sinY*cosX))) + (a*cu*cv*cosPowers);

    /////////////////////////////////
    // Equation of y
    // y = (c * sin(u) * sin(v)) + (a * sin(u) * sin(v))*((cos(b * cos(u) * sin(v)) * cos(b * sin(u) * sin(v)) * cos(b * cos(v)))^p)
    // with intermediates
  //  cosVertexPosition.y = (c*su*sv) + (a*su*sv * cosPowers);

    // (d)/(du)(y = c sin(u) sin(v)) = c cos(u) sin(v)
    // (d)/(du)(y = a sin(u) sin(v) (cos(b cos(u) sin(v)) cos(b sin(u) sin(v)) cos(b cos(v)))^p) = (0 = a p sin(u) sin(v) (b sin(u) sin(v) cos(b cos(v)) cos(b sin(u) sin(v)) sin(b cos(u) sin(v)) - b cos(u) sin(v) cos(b cos(v)) sin(b sin(u) sin(v)) cos(b cos(u) sin(v))) (cos(b cos(v)) cos(b cos(u) sin(v)) cos(b sin(u) sin(v)))^(p - 1) + a cos(u) sin(v) (cos(b cos(v)) cos(b cos(u) sin(v)) cos(b sin(u) sin(v)))^p)

        // differentiate with respect to u
        dydu = (c*su*cv) + (a*p*su*sv*cosPowersLess* ((b*su*sv*cosZ*cosY*sinX) - (b*cu*sv*cosZ*sinY*cosX))) + (a*cu*sv*cosPowers);

    // (d)/(dv)(y = c sin(u) sin(v)) = c sin(u) cos(v)
    // (d)/(dv)(y = a sin(u) sin(v) (cos(b cos(u) sin(v)) cos(b sin(u) sin(v)) cos(b cos(v)))^p) = (0 = a p sin(u) sin(v) (b sin(v) sin(b cos(v)) cos(b cos(u) sin(v)) cos(b sin(u) sin(v)) - b cos(u) cos(v) cos(b cos(v)) cos(b sin(u) sin(v)) sin(b cos(u) sin(v)) - b sin(u) cos(v) cos(b cos(v)) sin(b sin(u) sin(v)) cos(b cos(u) sin(v))) (cos(b cos(v)) cos(b cos(u) sin(v)) cos(b sin(u) sin(v)))^(p - 1) + a sin(u) cos(v) (cos(b cos(v)) cos(b cos(u) sin(v)) cos(b sin(u) sin(v)))^p)

        // differentiate with respect to v
        dydv = (c*su*cv) + (a*p*su*sv*cosPowersLess*(b*sv*sinZ*cosX*cosY - b*cu*cv*cosZ*cosY*sinX - b*su*cv*cosZ*sinY*cosX)) + (a*su*cv*cosPowers);

    /////////////////////////////////
    // Equation of z
    // z = (c * cos(v)) + (a * cos(v))*((cos(b * cos(u) * sin(v)) * cos(b * sin(u) * sin(v)) * cos(b * cos(v)))^p)
    // with intermediates
 //   cosVertexPosition.z = (c*cv) + (a*cv*cosPowers);

    // (d)/(du)(z = c cos(v)) = 0
    // (d)/(du)(x = a cos(v) (cos(b cos(u) sin(v)) cos(b sin(u) sin(v)) cos(b cos(v)))^p) = a b p sin(v) cos(v) (cos(b cos(v)) cos(b cos(u) sin(v)) cos(b sin(u) sin(v)))^p (sin(u) tan(b cos(u) sin(v)) - cos(u) tan(b sin(u) sin(v)))

        // differentiate with respect to u
        dzdu = a*b*p*sv*cv*cosPowers * (su*tanX - cu*tanY);

    // (d)/(dv)(z = c cos(v)) = -c sin(v)
    // (d)/(dv)(z = a cos(v) (cos(b cos(u) sin(v)) cos(b sin(u) sin(v)) cos(b cos(v)))^p) = (0 = a p cos(v) (b sin(v) sin(b cos(v)) cos(b cos(u) sin(v)) cos(b sin(u) sin(v)) - b cos(u) cos(v) cos(b cos(v)) cos(b sin(u) sin(v)) sin(b cos(u) sin(v)) - b sin(u) cos(v) cos(b cos(v)) sin(b sin(u) sin(v)) cos(b cos(u) sin(v))) (cos(b cos(v)) cos(b cos(u) sin(v)) cos(b sin(u) sin(v)))^(p - 1) - a sin(v) (cos(b cos(v)) cos(b cos(u) sin(v)) cos(b sin(u) sin(v)))^p)

        // differentiate with respect to v
        dzdv = (-1*c*sv) + (a*p*cv*cosPowersLess*(b*sv*sinZ*cosX*cosY - b*cu*cv*cosZ*cosY*sinX - b*su*cv*cosZ*sinY*cosX)) - (a*sv*cosPowers);

    /////////////////////////////////
    // Cross product of (dxdu, dydu, dzdu) X (dxdv, dydv, dzdv) = normal vector (will need normalizing)

    float3 cosNorms = cross(float3(dxdu, dydu, dzdu), float3(dxdv, dydv, dzdv));
    cosNorms = normalize(cosNorms);

	// Calculate normals for sin configuration
	//sinNormal.x = -1 / ((sinSev + 1) + (powers * sinSev * vxr * (1 / tan(vxr))));
	//sinNormal.y = -1 / ((sinSev + 1) + (powers * sinSev * vyr * (1 / tan(vyr))));
	//sinNormal.z = -1 / ((sinSev + 1) + (powers * sinSev * vzr * (1 / tan(vzr))));

	//// Get unit normal
	//sinNormal = normalize(sinNormal);

	//sinColourModifier.x *= sinWarp;
	//sinColourModifier.y *= sinWarp;
	//sinColourModifier.z *= sinWarp;

	//// Calculate cos wave surface modifier
	float cosWarp = 1.0f;
	float cosvx = cos(vxr);
	float cosvy = cos(vyr);
	float cosvz = cos(vzr);
	float cosvxyz = cosvx * cosvy * cosvz;

    

	for (int j = 0; j < powers; j++)
	{
		cosWarp *= cosvxyz;
	}

	float cosSev = cosWarp * severity;

	cosVertexPosition.x += radialVector.x * cosSev;
	cosVertexPosition.y += radialVector.y * cosSev;
	cosVertexPosition.z += radialVector.z * cosSev;

	//// normal = -1 /( s (cos(r x) cos(r y) cos(r z))^p - p r s x tan(r x) (cos(r x) cos(r y) cos(r z))^p + 1)

	//cosNormal.x = -1 / ((cosSev + 1) - (powers * cosSev * vxr * (tan(vxr))));
	//cosNormal.y = -1 / ((cosSev + 1) - (powers * cosSev * vyr * (tan(vyr))));
	//cosNormal.z = -1 / ((cosSev + 1) - (powers * cosSev * vzr * (tan(vzr))));

	//cosNormal = normalize(cosNormal);

	//cosColourModifier.x *= cosWarp;
	//cosColourModifier.y *= cosWarp;
	//cosColourModifier.z *= cosWarp;
	
	// Lerp between cos function and sin function
	if (targetSin)
	{
		vertexPosition.x = lerp(cosVertexPosition.x, sinVertexPosition.x, lerpAmount);
		vertexPosition.y = lerp(cosVertexPosition.y, sinVertexPosition.y, lerpAmount);
		vertexPosition.z = lerp(cosVertexPosition.z, sinVertexPosition.z, lerpAmount);
		 
		normals.x = lerp(cosNorms.x, sinNorms.x, lerpAmount);
        normals.y = lerp(cosNorms.y, sinNorms.y, lerpAmount);
        normals.z = lerp(cosNorms.z, sinNorms.z, lerpAmount);

		colourModifier.x = lerp(cosColourModifier.x, sinColourModifier.x, lerpAmount);
		colourModifier.y = lerp(cosColourModifier.y, sinColourModifier.y, lerpAmount);
		colourModifier.z = lerp(cosColourModifier.z, sinColourModifier.z, lerpAmount);
	}
	else
	{
		vertexPosition.x = lerp(sinVertexPosition.x, cosVertexPosition.x, 1 - lerpAmount);
		vertexPosition.y = lerp(sinVertexPosition.y, cosVertexPosition.y, 1 - lerpAmount);
		vertexPosition.z = lerp(sinVertexPosition.z, cosVertexPosition.z, 1 - lerpAmount);

        normals.x = lerp(sinNorms.x, cosNorms.x, 1 - lerpAmount);
        normals.y = lerp(sinNorms.y, cosNorms.y, 1 - lerpAmount);
        normals.z = lerp(sinNorms.z, cosNorms.z, 1 - lerpAmount);

		colourModifier.x = lerp(sinColourModifier.x, cosColourModifier.x, 1 - lerpAmount);
		colourModifier.y = lerp(sinColourModifier.y, cosColourModifier.y, 1 - lerpAmount);
		colourModifier.z = lerp(sinColourModifier.z, cosColourModifier.z, 1 - lerpAmount);
	}

	colourModifier *= 2;

    //vertexPosition = cosVertexPosition;
    //normals = cosNorms;

    //vertexPosition = sinVertexPosition;
    //normals = sinNorms;

    float tone = radius / (length(vertexPosition) * 2.0f);

	//colourModifier = normalize(colourModifier);
	colourModifier.x = clamp(colourModifier.x, 0.0f, 1.0f);
	colourModifier.y = clamp(colourModifier.y, 0.0f, 1.0f);
	colourModifier.z = clamp(colourModifier.z, 0.0f, 1.0f);
	colourModifier.a = 1.0f;

    // Calculate the position of the new vertex against the world, view, and projection matrices.
    output.position = float4(vertexPosition, 1.0f);
    //output.position = mul(float4(vertexPosition, 1.0f), worldMatrix);
    //output.position = mul(output.position, viewMatrix);
    //output.position = mul(output.position, projectionMatrix);

    // Calculate the position of the vertice as viewed by the light source.
    output.lightViewPosition = mul(float4(vertexPosition, 1.0f), worldMatrix);
    output.lightViewPosition = mul(output.lightViewPosition, lightView);
    output.lightViewPosition = mul(output.lightViewPosition, lightProjection);

    output.position3D = mul(float4(vertexPosition, 1.0f), worldMatrix);

    output.lightPos = normalize(lightPosition.xyz - output.position3D.xyz);
	// Making the changes here uses the position in screen space

    // Send the input color into the pixel shader.
    colourModifier.x = 1 - (baseColour.x + saturate(tone));
    colourModifier.y = 1 - (baseColour.y + saturate(tone));
    colourModifier.z = 1 - (baseColour.z + saturate(tone));
    colourModifier.w = 1.0f;
    
    colourModifier = saturate(colourModifier);

    output.colour = colourModifier;

    output.normal = normals;

    return output;
}

