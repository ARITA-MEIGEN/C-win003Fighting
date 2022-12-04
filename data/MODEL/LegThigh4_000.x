xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 122;
 2.18075;5.84259;2.94132;,
 2.43595;5.50543;3.36615;,
 2.80549;5.50543;2.99661;,
 2.46548;5.84259;2.65659;,
 2.72998;5.84259;2.39208;,
 3.07000;5.50543;2.73210;,
 3.43954;5.50543;2.36256;,
 3.01470;5.84259;2.10736;,
 2.78378;5.16568;3.71398;,
 3.15332;5.16568;3.34444;,
 3.41783;5.16568;3.07993;,
 3.78737;5.16568;2.71039;,
 2.68851;4.68274;3.63646;,
 3.05600;4.67590;3.24712;,
 3.32051;4.67590;2.98261;,
 3.70985;4.68274;2.61512;,
 2.73027;4.23673;3.66047;,
 3.09981;4.23673;3.29093;,
 3.36432;4.23673;3.02642;,
 3.73386;4.23673;2.65688;,
 2.82838;3.81383;3.75858;,
 3.19792;3.81383;3.38904;,
 3.46243;3.81383;3.12453;,
 3.83197;3.81383;2.75499;,
 2.92649;3.39093;3.85669;,
 3.29603;3.39093;3.48715;,
 3.56054;3.39093;3.22264;,
 3.93008;3.39093;2.85310;,
 3.02459;2.96803;3.95480;,
 3.39413;2.96803;3.58525;,
 3.65864;2.96803;3.32074;,
 4.02818;2.96803;2.95120;,
 2.87932;5.03731;1.80234;,
 2.69918;5.73076;1.79184;,
 2.66188;5.03916;1.97440;,
 2.55450;4.93092;1.47751;,
 2.35950;4.93199;1.67202;,
 3.38354;4.95526;2.30655;,
 2.91810;4.82519;1.84111;,
 2.59670;4.74903;1.51971;,
 3.27082;4.54055;2.25174;,
 3.10153;4.46626;2.00184;,
 2.69109;4.32613;1.62153;,
 3.37336;4.10895;2.29637;,
 3.10899;4.01526;2.03201;,
 2.79291;3.90323;1.71593;,
 3.47146;3.68605;2.39448;,
 3.56957;3.26316;2.49259;,
 3.20710;3.59236;2.13012;,
 3.30520;3.16946;2.22822;,
 3.01400;3.05743;1.88727;,
 2.89102;3.48033;1.81403;,
 3.66768;2.84026;2.59070;,
 2.18471;4.93199;1.84681;,
 2.18471;4.93199;1.84681;,
 1.92020;4.93199;2.11132;,
 1.92020;4.93199;2.11132;,
 1.55090;4.93092;2.48111;,
 1.74541;4.93199;2.28611;,
 2.22715;4.74903;1.88925;,
 1.96264;4.74903;2.15376;,
 1.59310;4.74903;2.52330;,
 2.32526;4.32613;1.98736;,
 2.06075;4.32613;2.25187;,
 1.69492;4.32613;2.61770;,
 2.42337;3.90323;2.08547;,
 2.15886;3.90323;2.34998;,
 1.78932;3.90323;2.71952;,
 2.52147;3.48033;2.18357;,
 2.25696;3.48033;2.44808;,
 1.88742;3.48033;2.81762;,
 2.61958;3.05743;2.28168;,
 2.35507;3.05743;2.54619;,
 1.96066;3.05743;2.94061;,
 3.08723;2.63454;2.01025;,
 2.71769;2.63453;2.37979;,
 2.45318;2.63453;2.64430;,
 2.08364;2.63453;3.01384;,
 1.87573;5.03731;2.80593;,
 2.04779;5.03916;2.58849;,
 1.86523;5.73076;2.62579;,
 1.91451;4.82519;2.84470;,
 2.37994;4.95526;3.31014;,
 2.07523;4.46626;3.02814;,
 2.32513;4.54055;3.19744;,
 2.10540;4.01526;3.03560;,
 2.36976;4.10895;3.29996;,
 2.46787;3.68605;3.39807;,
 2.30161;3.16946;3.23182;,
 2.20351;3.59236;3.13371;,
 2.56598;3.26316;3.49618;,
 2.66409;2.84026;3.59429;,
 2.22258;5.03916;2.41370;,
 2.48709;5.03916;2.14919;,
 2.14995;5.73076;2.34107;,
 2.41446;5.73076;2.07656;,
 2.39972;2.74656;3.32992;,
 2.35272;2.76623;2.99619;,
 3.06958;2.76623;2.27934;,
 3.40331;2.74656;2.32633;,
 2.84713;3.37733;3.49060;,
 3.56399;3.37733;2.77374;,
 2.97449;3.90323;1.53435;,
 3.29056;4.01526;1.85043;,
 3.07259;3.48033;1.63246;,
 3.38868;3.59236;1.94854;,
 3.20326;3.05250;1.71383;,
 3.49446;3.16453;2.05479;,
 3.27649;2.62960;1.83682;,
 3.59257;2.74163;2.15290;,
 1.92383;4.01526;3.21717;,
 1.60775;3.90323;2.90109;,
 2.02193;3.59236;3.31528;,
 1.70585;3.48033;2.99920;,
 2.12818;3.16453;3.42107;,
 1.78723;3.05250;3.12987;,
 2.22629;2.74163;3.51919;,
 1.91021;2.62960;3.20310;,
 3.72703;1.95359;2.93679;,
 4.22144;2.56470;3.43119;,
 3.01018;1.95359;3.65364;,
 3.50458;2.56470;4.14805;;
 
 122;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;1,8,9,2;,
 4;5,10,11,6;,
 4;8,12,13,9;,
 4;10,14,15,11;,
 4;12,16,17,13;,
 4;14,18,19,15;,
 4;16,20,21,17;,
 4;18,22,23,19;,
 4;20,24,25,21;,
 4;22,26,27,23;,
 4;24,28,29,25;,
 4;26,30,31,27;,
 4;7,6,32,33;,
 4;34,32,35,36;,
 4;6,11,37,32;,
 4;32,38,39,35;,
 4;11,15,40,37;,
 4;38,41,42,39;,
 4;15,19,43,40;,
 4;41,44,45,42;,
 4;19,23,46,43;,
 4;23,27,47,46;,
 4;48,49,50,51;,
 4;27,31,52,47;,
 4;36,35,53,54;,
 4;55,56,57,58;,
 4;35,39,59,53;,
 4;56,60,61,57;,
 4;39,42,62,59;,
 4;60,63,64,61;,
 4;42,45,65,62;,
 4;63,66,67,64;,
 4;45,51,68,65;,
 4;66,69,70,67;,
 4;51,50,71,68;,
 4;69,72,73,70;,
 4;50,74,75,71;,
 4;72,76,77,73;,
 4;58,57,78,79;,
 4;80,78,1,0;,
 4;57,61,81,78;,
 4;78,82,8,1;,
 4;61,64,83,81;,
 4;82,84,12,8;,
 4;64,67,85,83;,
 4;84,86,16,12;,
 4;86,87,20,16;,
 4;70,73,88,89;,
 4;87,90,24,20;,
 4;90,91,28,24;,
 4;58,79,92,55;,
 4;54,93,34,36;,
 4;80,0,3,94;,
 4;95,4,7,33;,
 4;96,77,76,97;,
 4;98,75,74,99;,
 4;28,91,100,29;,
 4;30,101,52,31;,
 4;38,37,40,41;,
 4;41,40,43,44;,
 4;44,43,46,48;,
 4;48,46,47,49;,
 4;49,47,52,99;,
 4;83,84,82,81;,
 4;85,86,84,83;,
 4;89,87,86,85;,
 4;88,90,87,89;,
 4;96,91,90,88;,
 4;79,80,94,92;,
 4;93,95,33,34;,
 4;97,100,91,96;,
 4;99,52,101,98;,
 4;4,3,2,5;,
 4;5,2,9,10;,
 4;10,9,13,14;,
 4;14,13,17,18;,
 4;18,17,21,22;,
 4;22,21,25,26;,
 4;26,25,29,30;,
 4;53,56,55,54;,
 4;59,60,56,53;,
 4;62,63,60,59;,
 4;65,66,63,62;,
 4;68,69,66,65;,
 4;71,72,69,68;,
 4;75,76,72,71;,
 4;54,55,92,93;,
 4;95,94,3,4;,
 4;98,97,76,75;,
 4;30,29,100,101;,
 4;93,92,94,95;,
 3;34,33,32;,
 3;32,37,38;,
 3;78,80,79;,
 3;81,82,78;,
 4;102,45,44,103;,
 4;104,51,45,102;,
 4;105,48,51,104;,
 4;103,44,48,105;,
 4;103,105,104,102;,
 4;106,50,49,107;,
 4;108,74,50,106;,
 4;109,99,74,108;,
 4;107,49,99,109;,
 4;107,109,108,106;,
 4;110,85,67,111;,
 4;112,89,85,110;,
 4;113,70,89,112;,
 4;111,67,70,113;,
 4;111,113,112,110;,
 4;114,88,73,115;,
 4;116,96,88,114;,
 4;117,77,96,116;,
 4;115,73,77,117;,
 4;115,117,116,114;,
 4;118,98,101,119;,
 4;120,97,98,118;,
 4;121,100,97,120;,
 4;119,101,100,121;,
 4;119,121,120,118;;
 
 MeshMaterialList {
  3;
  122;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2;;
  Material {
   0.592800;0.636800;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\OC\\Documents\\メタセコ\\secret\\TEXTURE\\22274600.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\OC\\Downloads\\9brngu000000g2d2.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\OC\\Documents\\メタセコ\\secret\\TEXTURE\\22274600.jpg";
   }
  }
 }
 MeshNormals {
  175;
  0.128660;0.983307;0.128661;,
  0.128659;0.983307;0.128661;,
  0.128658;0.983308;0.128659;,
  0.403904;0.820806;0.403906;,
  0.403903;0.820806;0.403907;,
  0.403903;0.820806;0.403906;,
  0.401848;0.822821;0.401850;,
  0.401848;0.822820;0.401852;,
  0.401851;0.822818;0.401853;,
  0.715219;-0.069307;0.695455;,
  0.710397;-0.068079;0.700501;,
  0.700500;-0.068080;0.710397;,
  0.695420;0.217969;0.684748;,
  0.692077;0.222290;0.686744;,
  0.686744;0.222290;0.692077;,
  0.671870;0.311734;0.671872;,
  0.671871;0.311734;0.671872;,
  0.671872;0.311734;0.671871;,
  0.671872;0.311733;0.671871;,
  0.671872;0.311732;0.671871;,
  0.671872;0.311731;0.671872;,
  0.671874;0.311727;0.671871;,
  0.671874;0.311726;0.671872;,
  0.671874;0.311724;0.671873;,
  0.128658;0.983308;0.128657;,
  0.640231;0.175631;-0.747836;,
  -0.162280;0.971043;-0.175330;,
  0.403906;0.820806;0.403904;,
  0.692793;0.044060;-0.719789;,
  0.401853;0.822817;0.401853;,
  0.714890;-0.000704;-0.699237;,
  0.718803;0.005892;-0.695189;,
  0.695457;-0.069308;0.715217;,
  0.719869;0.017881;-0.693880;,
  0.730297;0.014679;-0.682972;,
  0.684750;0.217969;0.695418;,
  0.712118;0.018563;-0.701814;,
  0.722673;0.011703;-0.691091;,
  0.671872;0.311735;0.671870;,
  0.707108;-0.000000;-0.707105;,
  0.713273;0.016763;-0.700686;,
  0.671872;0.311732;0.671872;,
  0.707108;0.000000;-0.707106;,
  0.713273;0.016765;-0.700686;,
  0.671873;0.311724;0.671874;,
  0.707108;-0.000001;-0.707106;,
  -0.026212;-0.850697;0.525003;,
  -0.672554;-0.308761;-0.672561;,
  -0.672005;-0.311156;-0.672005;,
  -0.152652;0.971980;-0.178751;,
  -0.672100;-0.310742;-0.672102;,
  -0.671916;-0.311538;-0.671917;,
  0.699973;0.001098;-0.714168;,
  -0.671872;-0.311731;-0.671872;,
  -0.671871;-0.311730;-0.671873;,
  0.694298;-0.005353;-0.719667;,
  -0.671871;-0.311734;-0.671871;,
  -0.671871;-0.311735;-0.671872;,
  0.695786;-0.012902;-0.718134;,
  -0.671871;-0.311730;-0.671873;,
  -0.671872;-0.311730;-0.671872;,
  0.724832;0.050275;-0.687088;,
  -0.671872;-0.311728;-0.671873;,
  -0.671873;-0.311729;-0.671872;,
  -0.671871;-0.311736;-0.671871;,
  -0.671872;-0.311735;-0.671870;,
  -0.658949;-0.313017;-0.683964;,
  -0.671871;-0.311736;-0.671871;,
  -0.671872;-0.311734;-0.671871;,
  -0.175328;0.971043;-0.162280;,
  -0.747838;0.175629;0.640229;,
  -0.671939;-0.311443;-0.671939;,
  -0.178745;0.971981;-0.152653;,
  -0.671872;-0.311730;-0.671873;,
  -0.695191;0.005893;0.718801;,
  -0.699239;-0.000701;0.714887;,
  -0.671871;-0.311735;-0.671871;,
  -0.682975;0.014677;0.730294;,
  -0.693883;0.017878;0.719865;,
  -0.671874;-0.311727;-0.671872;,
  -0.691091;0.011702;0.722673;,
  -0.701817;0.018560;0.712116;,
  -0.671874;-0.311726;-0.671872;,
  -0.700685;0.016767;0.713274;,
  -0.707106;0.000000;0.707108;,
  -0.679957;-0.312598;-0.663281;,
  -0.700685;0.016767;0.713274;,
  -0.707106;0.000002;0.707108;,
  -0.683964;-0.313012;-0.658951;,
  0.525004;-0.850696;-0.026215;,
  -0.707106;0.000002;0.707107;,
  -0.171891;0.970004;-0.171891;,
  -0.171890;0.970004;-0.171891;,
  -0.171903;0.969999;-0.171903;,
  -0.171904;0.969999;-0.171903;,
  0.608852;-0.789944;0.072708;,
  0.072711;-0.789945;0.608851;,
  0.515116;-0.634653;-0.576082;,
  -0.576081;-0.634654;0.515116;,
  -0.171896;0.970002;-0.171894;,
  0.171894;-0.970002;0.171896;,
  -0.149694;0.970126;-0.190911;,
  0.151149;-0.969587;0.192498;,
  -0.671874;-0.311721;-0.671875;,
  -0.645822;-0.314199;-0.695839;,
  -0.171898;0.970002;-0.171894;,
  0.171896;-0.970002;0.171895;,
  0.671874;0.311734;0.671868;,
  -0.190909;0.970127;-0.149692;,
  0.192495;-0.969587;0.151148;,
  0.696526;0.311542;0.646369;,
  -0.465334;-0.752947;-0.465333;,
  0.707107;-0.000001;-0.707107;,
  -0.707107;-0.000002;0.707107;,
  0.691852;-0.258209;0.674292;,
  0.686322;-0.264381;0.677543;,
  0.677543;-0.264380;0.686323;,
  0.674293;-0.258207;0.691851;,
  0.676804;0.088225;-0.730857;,
  0.691833;-0.012088;-0.721956;,
  0.707107;-0.000000;-0.707107;,
  0.672264;0.001006;-0.740310;,
  0.687831;0.013096;-0.725753;,
  0.707109;0.000001;-0.707105;,
  0.707108;0.000000;-0.707106;,
  0.707108;-0.000002;-0.707105;,
  -0.672214;-0.310248;-0.672217;,
  -0.671873;-0.311731;-0.671871;,
  -0.671871;-0.311735;-0.671871;,
  -0.671872;-0.311727;-0.671874;,
  -0.671872;-0.311725;-0.671874;,
  -0.663280;-0.312601;-0.679957;,
  -0.719790;0.044062;0.692792;,
  -0.730858;0.088229;0.676803;,
  -0.707106;-0.000002;0.707108;,
  -0.714167;0.001099;0.699974;,
  -0.721956;-0.012081;0.691834;,
  -0.719665;-0.005352;0.694300;,
  -0.740310;0.001006;0.672265;,
  -0.718131;-0.012903;0.695789;,
  -0.725757;0.013093;0.687827;,
  -0.707109;-0.000002;0.707104;,
  -0.687092;0.050278;0.724829;,
  -0.707109;0.000000;0.707105;,
  -0.707109;0.000002;0.707105;,
  -0.171903;0.969999;-0.171903;,
  -0.171904;0.969999;-0.171903;,
  0.320254;-0.934263;0.156811;,
  0.435729;-0.851034;0.293055;,
  0.293057;-0.851035;0.435727;,
  0.156813;-0.934263;0.320253;,
  0.158833;-0.709636;-0.686432;,
  -0.166997;-0.770330;-0.615389;,
  -0.615390;-0.770329;-0.166998;,
  -0.686435;-0.709634;0.158830;,
  0.707108;-0.000001;-0.707106;,
  -0.707104;0.000002;0.707110;,
  -0.736927;-0.159100;-0.656982;,
  -0.699434;-0.146913;-0.699435;,
  -0.699434;-0.146913;-0.699435;,
  -0.656984;-0.159101;-0.736926;,
  0.000000;0.000000;0.000000;,
  -0.473640;-0.742517;-0.473639;,
  -0.612325;-0.170754;-0.771946;,
  -0.771945;-0.170753;-0.612328;,
  0.671870;0.311739;0.671870;,
  0.707105;0.000001;-0.707108;,
  0.646376;0.311541;0.696520;,
  0.738505;-0.026387;-0.673731;,
  -0.671875;-0.311720;-0.671874;,
  -0.707107;-0.000000;0.707107;,
  -0.695843;-0.314194;-0.645822;,
  -0.673735;-0.026388;0.738502;,
  0.465336;0.752944;0.465336;,
  0.465337;-0.752944;0.465335;;
  122;
  4;0,3,4,1;,
  4;2,5,27,24;,
  4;3,6,7,4;,
  4;5,8,29,27;,
  4;114,9,10,115;,
  4;116,11,32,117;,
  4;9,12,13,10;,
  4;11,14,35,32;,
  4;12,15,16,13;,
  4;14,17,38,35;,
  4;15,18,19,16;,
  4;17,20,41,38;,
  4;18,21,22,19;,
  4;20,23,44,41;,
  4;25,118,28,25;,
  4;26,49,49,26;,
  4;118,119,30,28;,
  4;28,31,52,120;,
  4;119,121,33,30;,
  4;31,34,55,52;,
  4;121,122,36,33;,
  4;34,37,58,55;,
  4;122,123,39,36;,
  4;123,124,42,39;,
  4;40,43,61,61;,
  4;124,125,45,42;,
  4;47,126,50,47;,
  4;48,51,71,48;,
  4;126,127,53,50;,
  4;51,54,73,71;,
  4;127,128,56,53;,
  4;54,57,76,73;,
  4;128,129,59,56;,
  4;57,60,79,76;,
  4;129,130,62,59;,
  4;60,63,82,79;,
  4;130,131,64,62;,
  4;63,65,85,82;,
  4;131,66,67,64;,
  4;65,68,88,85;,
  4;69,72,72,69;,
  4;70,132,133,70;,
  4;134,135,74,132;,
  4;132,75,136,133;,
  4;135,137,77,74;,
  4;75,78,138,136;,
  4;137,139,80,77;,
  4;78,81,140,138;,
  4;81,84,141,140;,
  4;142,142,86,83;,
  4;84,87,143,141;,
  4;87,90,144,143;,
  4;69,69,91,91;,
  4;92,92,26,26;,
  4;145,0,1,93;,
  4;94,2,24,146;,
  4;89,147,148,95;,
  4;96,149,150,46;,
  4;151,97,97,152;,
  4;153,98,98,154;,
  4;31,30,33,34;,
  4;34,33,36,37;,
  4;37,36,39,40;,
  4;40,39,42,43;,
  4;43,42,45,155;,
  4;77,78,75,74;,
  4;80,81,78,77;,
  4;83,84,81,80;,
  4;86,87,84,83;,
  4;156,90,87,86;,
  4;157,157,158,158;,
  4;159,159,160,160;,
  4;95,97,97,89;,
  4;46,98,98,96;,
  4;2,1,4,5;,
  4;5,4,7,8;,
  4;116,115,10,11;,
  4;11,10,13,14;,
  4;14,13,16,17;,
  4;17,16,19,20;,
  4;20,19,22,23;,
  4;161,161,161,161;,
  4;53,54,51,50;,
  4;56,57,54,53;,
  4;59,60,57,56;,
  4;62,63,60,59;,
  4;64,65,63,62;,
  4;67,68,65,64;,
  4;92,91,91,92;,
  4;94,93,1,2;,
  4;96,95,148,149;,
  4;153,152,162,162;,
  4;159,158,158,159;,
  3;160,160,163;,
  3;28,30,31;,
  3;164,157,157;,
  3;74,75,132;,
  4;99,99,99,99;,
  4;103,130,129,103;,
  4;100,100,100,100;,
  4;165,165,165,165;,
  4;166,166,166,166;,
  4;101,101,101,101;,
  4;104,66,131,104;,
  4;102,46,150,102;,
  4;167,167,167,167;,
  4;168,168,168,168;,
  4;105,105,105,105;,
  4;107,107,107,107;,
  4;106,106,106,106;,
  4;169,79,82,169;,
  4;170,170,170,170;,
  4;108,108,108,108;,
  4;110,110,110,110;,
  4;109,147,89,109;,
  4;171,85,88,171;,
  4;172,172,172,172;,
  4;112,112,112,112;,
  4;111,111,111,111;,
  4;113,113,113,113;,
  4;173,173,173,173;,
  4;174,174,174,174;;
 }
}
