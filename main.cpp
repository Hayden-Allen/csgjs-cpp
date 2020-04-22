//#define CSGJSCPP_REAL double
#define CSGJSCPP_IMPLEMENTATION
#include "csgjs.h"

#include <fstream>

#if defined(WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif
#include <iostream>

namespace exunit {

bool modeltoply(const char *filename, const csgjs_model &model) {

    std::fstream stream(filename, std::ios_base::out);
    if (stream.is_open()) {
        stream << "ply\n";
        stream << "format ascii 1.0\n";

        // define the vertex elements
        stream << "element vertex " << model.vertices.size() << '\n';
        stream << "property float x\n";
        stream << "property float y\n";
        stream << "property float z\n";
        stream << "property float nx\n";
        stream << "property float ny\n";
        stream << "property float nz\n";
        stream << "property uchar red\n";
        stream << "property uchar green\n";
        stream << "property uchar blue\n";

        // define the face elements
        stream << "element face " << model.indices.size() / 3 << '\n';
        stream << "property list uchar int32 vertex_indices\n";
        stream << "end_header\n";

        for (const auto &v : model.vertices) {
            stream << v.pos.x << " " << v.pos.y << " " << v.pos.z << " " << v.normal.x << " " << v.normal.y << " "
                   << v.normal.z << " " << (int)(v.col.x * 255) << " " << (int)(v.col.y * 255) << " "
                   << (int)(v.col.z * 255) << " " << '\n';
        }

        for (int idx = 2; idx < model.indices.size(); idx += 3) {
            stream << "3 " << model.indices[idx - 2] << " " << model.indices[idx - 1] << " " << model.indices[idx - 0]
                   << '\n';
        }
        return true;
    }

    return false;
}

struct Timer {
    Timer() {
#if defined(WIN32)
        QueryPerformanceCounter(&mStartingTime);
#endif
    }

    /* get the elapsed micro seconds */
    uint64_t GetElapsedMS() const {
#if defined(WIN32)
        LARGE_INTEGER endtime;
        QueryPerformanceCounter(&endtime);

        LARGE_INTEGER elapsed;
        elapsed.QuadPart = endtime.QuadPart - mStartingTime.QuadPart;

        elapsed.QuadPart *= 1000000;

        return elapsed.QuadPart / sFrequency.QuadPart;
#else
        return 0;
#endif
    }

    static void Init() {
#if defined(WIN32)
        QueryPerformanceFrequency(&sFrequency);
#endif
    }

#if defined(WIN32)
    static LARGE_INTEGER sFrequency;
    LARGE_INTEGER        mStartingTime;
#endif
};

#if defined(WIN32)
LARGE_INTEGER Timer::sFrequency;
#endif

} // namespace exunit

int main(int /*argc*/, char ** /*arvc*/) {

    exunit::Timer::Init();

    csgjs_vector red(1.0f, 0.0f, 0.0f);
    csgjs_vector green(0.0f, 1.0f, 0.0f);
    csgjs_vector blue(0.0f, 0.0f, 1.0f);
    csgjs_vector white(1.0f, 1.0f, 1.0f);

    /* output a series of PLY files to see if operations work. */

    {
        auto cube1 = csgsmodel_cube();
        auto cube2 = csgsmodel_cube({1, 0, 0}, {0.8f, 0.8f, 0.8f});

        auto model = csgjs_subtract(cube1, cube2);

        exunit::modeltoply("cube_subtract_cube.ply", model);
    }

    {
        auto cube1 = csgsmodel_cube();
        auto sphere = csgsmodel_sphere({0.5, 0, 0}, 0.8f);

        auto model = csgjs_subtract(cube1, sphere);

        exunit::modeltoply("cube_subtract_sphere.ply", model);
    }

    {
        auto cube1 = csgsmodel_cube();
        auto cylinder = csgsmodel_cylinder({0.0, -1, 0}, {0.0, 1.0, 0}, 0.8f);

        auto model = csgjs_subtract(cube1, cylinder);

        exunit::modeltoply("cube_subtract_cylinder.ply", model);
    }

    {
        auto a = csgsmodel_cube({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, white);
        auto b = csgsmodel_sphere({0, 0, 0}, 1.35f, white, 16);
        auto c = csgsmodel_cylinder({-1, 0, 0}, {1, 0, 0}, 0.7f, red);
        auto d = csgsmodel_cylinder({0, -1, 0}, {0, 1, 0}, 0.7f, green);
        auto e = csgsmodel_cylinder({0, 0, -1}, {0, 0, 1}, 0.7f, blue);

        // a.intersect(b).subtract(c.union(d).union(e))
        auto model = csgjs_subtract(csgjs_intersection(a, b), csgjs_union(csgjs_union(c, d), e));
        exunit::modeltoply("multiops.ply", model);
    }

    {
        // clang-format off
		// gourd model taken from the JS website and converted to C++ lists.
		csgjs_vector vertices[] = { {-0.238769f,1.31038f,0.130013f},{-0.275829f,1.258256f,0.123646f},{-0.267489f,1.347437f,0.159127f},{-0.312866f,1.222713f,0.146236f},{-0.301897f,1.383255f,0.199258f},{-0.344139f,1.184785f,0.31619f},{-0.327314f,1.347217f,0.335997f},{-0.339365f,1.160645f,0.250339f},{-0.329337f,1.387289f,0.302972f},{-0.319937f,1.201023f,0.353361f},{-0.307267f,1.295644f,0.364349f},{-0.287786f,1.249347f,0.382309f},{-0.210343f,1.353167f,0.153717f},{-0.233863f,1.344237f,0.37948f},{-0.211456f,1.381852f,0.351755f},{-0.25614f,1.295263f,0.39115f},{-0.008084f,-1.092125f,-0.524341f},{-0.079765f,-1.276867f,-0.525251f},{-0.160864f,-1.127278f,-0.743484f},{-0.100013f,-0.991044f,-0.702817f},{-0.020061f,-1.233439f,-0.301325f},{-0.079908f,-1.350904f,-0.34107f},{-0.101691f,-1.206887f,-0.048767f},{-0.151982f,-1.365511f,-0.156924f},{-0.260128f,-1.18858f,0.10989f},{-0.317296f,-1.321268f,0.059442f},{-0.222318f,-1.370571f,-0.625884f},{-0.284112f,-1.207877f,-0.803651f},{-0.198086f,-1.452216f,-0.388696f},{-0.312712f,-1.481223f,-0.16626f},{-0.446866f,-1.408121f,0.03031f},{-0.380914f,-1.432361f,-0.67131f},{-0.426185f,-1.253856f,-0.832658f},{-0.343927f,-1.52964f,-0.435967f},{-0.471213f,-1.542748f,-0.2127f},{-0.584034f,-1.449947f,-0.01689f},{-0.596105f,-0.676846f,-0.912022f},{-0.398821f,-0.97836f,-0.883308f},{-0.603971f,-0.97305f,-0.944293f},{-0.658441f,-1.233176f,-0.874621f},{-0.70941f,-1.465943f,-0.715402f},{-0.664481f,-1.577168f,-0.510421f},{-0.789406f,-1.563823f,-0.30882f},{-0.816283f,-1.431081f,-0.057528f},{-0.596532f,-1.231038f,0.154397f},{-0.811768f,-1.23444f,0.134141f},{-0.848569f,-0.938325f,0.239828f},{0.076112f,-0.915648f,-0.205368f},{-0.031081f,-0.899476f,0.043125f},{-0.250946f,-0.960219f,0.194989f},{-0.698201f,-0.8975f,-0.959998f},{-0.829367f,-1.115095f,-0.914649f},{-0.961905f,-1.343162f,-0.788032f},{-1.047212f,-1.473044f,-0.560546f},{-1.060766f,-1.464113f,-0.285566f},{-1.000425f,-1.328387f,-0.033614f},{-0.910491f,-1.166207f,0.147025f},{-1.244579f,-0.817542f,-0.897341f},{-1.265987f,-0.680125f,-0.849049f},{-0.958686f,-0.671083f,-0.936756f},{-1.441736f,-0.892844f,-0.751704f},{-1.476803f,-0.736002f,-0.668586f},{-1.563984f,-0.919161f,-0.528758f},{-1.568166f,-0.792727f,-0.499131f},{-1.532626f,-1.003982f,-0.289692f},{-1.548949f,-0.824241f,-0.301825f},{-1.40789f,-1.01804f,-0.066056f},{-1.422332f,-0.872309f,-0.053167f},{-1.16522f,-0.933717f,0.133418f},{-1.125544f,-0.452879f,-0.791549f},{-1.327921f,-0.423786f,-0.605745f},{-1.453237f,-0.505555f,-0.407349f},{-1.414896f,-0.530424f,-0.163085f},{-1.289436f,-0.658454f,0.049306f},{-1.373761f,-1.05989f,-0.765859f},{-1.165772f,-0.923232f,-0.911346f},{-1.501004f,-1.093151f,-0.558277f},{-1.460103f,-1.165444f,-0.32704f},{-1.330072f,-1.126145f,-0.072888f},{-1.087594f,-1.072288f,-0.895093f},{-0.887495f,-0.824803f,-0.956534f},{-1.24878f,-1.199621f,-0.77081f},{-1.38718f,-1.245335f,-0.572554f},{-1.333668f,-1.303381f,-0.33953f},{-1.23866f,-1.25869f,-0.123415f},{-1.094028f,-1.084367f,0.106821f},{-1.219074f,-0.296241f,-0.563562f},{-1.062905f,-0.336014f,-0.72581f},{-1.351711f,-0.347697f,-0.349294f},{-1.309105f,-0.407813f,-0.103394f},{-1.216809f,-0.527492f,0.067413f},{-0.84739f,-0.478493f,-0.871017f},{-0.682049f,-0.407345f,-0.836246f},{-0.848035f,-0.223945f,-0.685936f},{-0.938204f,-0.089206f,-0.456067f},{-1.089632f,-0.135377f,-0.247638f},{-1.017758f,-0.191124f,0.000707f},{-1.034806f,-0.410382f,0.139909f},{-1.05115f,-0.74168f,0.204914f},{-0.939649f,-0.63213f,0.243874f},{-0.095604f,1.339214f,0.147758f},{-0.152735f,1.295742f,0.109956f},{-0.103491f,1.313798f,0.417882f},{-0.150856f,1.269496f,0.43226f},{-0.214332f,1.220212f,0.423598f},{-0.21917f,1.249857f,0.099542f},{-0.282464f,1.20504f,0.116446f},{-0.312976f,1.163166f,0.14504f},{-0.275115f,1.178548f,0.388598f},{-0.326649f,1.147169f,0.329668f},{-0.071178f,1.283392f,0.101555f},{0.019526f,1.226838f,0.08763f},{-0.053373f,1.17809f,0.041486f},{-0.121599f,1.216039f,0.061913f},{-0.032161f,1.337285f,0.165115f},{0.073156f,1.265403f,0.16182f},{-0.018012f,1.364667f,0.229834f},{0.103671f,1.284341f,0.252382f},{-0.000152f,1.355462f,0.32272f},{0.131402f,1.247816f,0.327638f},{-0.056022f,1.329951f,0.393053f},{0.075222f,1.250714f,0.395662f},{-0.082812f,1.270342f,0.440224f},{0.017953f,1.197729f,0.449576f},{-0.140085f,1.198131f,0.454925f},{-0.05573f,1.134205f,0.474345f},{-0.195843f,1.12508f,0.43955f},{-0.136482f,1.076299f,0.464207f},{-0.132083f,1.11888f,0.030271f},{-0.177895f,1.141988f,0.05225f},{-0.202444f,1.062628f,0.050841f},{-0.232109f,1.07965f,0.071906f},{-0.208696f,1.025945f,0.418942f},{-0.246788f,1.059545f,0.393382f},{-0.263251f,0.99313f,0.351414f},{-0.28646f,1.018558f,0.328217f},{0.178723f,1.025247f,0.043494f},{0.067486f,1.032837f,-0.001201f},{0.256142f,1.013628f,0.108355f},{0.189368f,0.931987f,0.448177f},{0.093254f,0.920481f,0.478689f},{-0.026876f,0.917199f,0.478803f},{-0.050991f,1.030898f,-0.008079f},{-0.162091f,1.01595f,0.024677f},{-0.139173f,0.923452f,0.441196f},{-0.229159f,0.935442f,0.368599f},{0.178615f,0.902994f,-0.005193f},{0.208851f,0.575596f,-0.076671f},{0.079666f,0.844414f,-0.055717f},{0.108365f,0.59977f,-0.109998f},{0.30095f,0.943161f,0.286432f},{0.339788f,0.456837f,0.176755f},{0.293729f,0.962459f,0.175527f},{0.25272f,0.912342f,0.383905f},{-0.018812f,0.723755f,0.445275f},{0.009515f,0.478676f,0.392872f},{0.116956f,0.463012f,0.390824f},{0.086177f,0.789771f,0.464854f},{-0.012905f,0.621737f,-0.10774f},{-0.020542f,0.776614f,-0.070253f},{-0.114203f,0.630137f,-0.07232f},{-0.120273f,0.716205f,-0.044794f},{-0.194696f,0.663801f,0.001951f},{-0.186344f,0.541393f,0.294949f},{-0.189505f,0.631209f,0.308188f},{-0.237735f,0.611278f,0.218928f},{-0.104273f,0.509757f,0.357058f},{-0.110989f,0.665531f,0.391598f},{0.187634f,0.503306f,-0.104833f},{0.098094f,0.175761f,-0.226601f},{0.000998f,0.277694f,-0.238337f},{0.062026f,0.514337f,-0.144302f},{0.260177f,0.51325f,-0.041983f},{0.093381f,0.387832f,0.374674f},{-0.005212f,0.04918f,0.299049f},{0.182486f,0.418067f,0.35326f},{-0.038491f,0.391209f,0.367017f},{-0.108481f,0.143551f,0.318657f},{-0.093242f,0.359871f,-0.207403f},{-0.067197f,0.506645f,-0.139743f},{-0.180447f,0.417467f,-0.141587f},{-0.164047f,0.50087f,-0.099437f},{-0.192079f,0.238747f,0.295504f},{-0.156288f,0.397492f,0.313533f},{-0.254501f,0.32669f,0.235316f},{-0.231372f,0.418343f,0.242808f},{-0.018721f,0.062427f,-0.342358f},{-0.05159f,-0.256393f,-0.498006f},{-0.200885f,-0.131264f,-0.545344f},{-0.183408f,0.098252f,-0.39431f},{0.121154f,0.01442f,-0.240378f},{0.056559f,-0.360625f,-0.406923f},{-0.145385f,-0.093036f,0.302699f},{-0.208053f,-0.440277f,0.278544f},{0.025258f,-0.103229f,0.24769f},{-0.066403f,-0.501031f,0.197232f},{-0.320169f,-0.069473f,0.3022f},{-0.376423f,-0.337284f,0.319618f},{-0.371038f,-0.018344f,-0.51817f},{-0.362488f,0.117923f,-0.388516f},{-0.536991f,0.059873f,-0.420071f},{-0.497225f,0.134107f,-0.328619f},{-0.533628f,-0.208458f,0.285073f},{-0.486342f,-0.033084f,0.241778f},{-0.658306f,-0.082717f,0.179094f},{-0.593078f,0.019406f,0.150839f},{-0.164214f,-0.300907f,-0.61347f},{-0.191216f,-0.497363f,-0.702402f},{-0.380443f,-0.351758f,-0.746189f},{-0.369116f,-0.199244f,-0.660672f},{0.007641f,-0.41592f,-0.489491f},{-0.007031f,-0.639152f,-0.570563f},{0.110824f,-0.469848f,-0.316991f},{0.076818f,-0.758917f,-0.414695f},{0.122126f,-0.60506f,-0.124373f},{-0.139698f,-0.584544f,0.224329f},{-0.129093f,-0.824521f,0.17198f},{0.023932f,-0.56703f,0.096322f},{-0.356297f,-0.516456f,0.314674f},{-0.320714f,-0.755525f,0.27547f},{-0.575167f,-0.429802f,0.324789f},{-0.560882f,-0.630013f,0.325689f},{-0.611999f,-0.203944f,-0.705899f},{-0.5691f,-0.120629f,-0.636786f},{-0.773349f,-0.485247f,0.290004f},{-0.74481f,-0.325481f,0.26564f},{-0.774548f,-0.082997f,-0.553307f},{-0.905997f,-0.275794f,0.156571f},{-0.129281f,0.245412f,-0.278136f},{-0.012722f,0.178724f,-0.285644f},{-0.242814f,0.293957f,-0.223491f},{-0.629432f,0.115561f,-0.263913f},{-0.341125f,0.319012f,-0.127248f},{-0.741735f,0.080538f,-0.125227f},{-0.399209f,0.31073f,-0.003678f},{-0.690149f,0.042138f,0.042986f},{-0.3903f,0.258954f,0.122643f},{-0.331124f,0.185607f,0.226218f},{-0.240913f,0.108356f,0.290548f},{-0.12951f,0.035368f,0.309252f},{0.091164f,0.109055f,-0.24903f},{0.168933f,0.095347f,-0.174875f},{-0.01384f,-0.019768f,0.285771f},{0.08622f,-0.006153f,0.246511f},{-0.287614f,0.447378f,0.051054f},{-0.248653f,0.597994f,0.094511f},{-0.249916f,0.461425f,-0.040844f},{-0.286759f,0.416295f,0.14636f},{-0.295257f,0.967508f,0.179217f},{0.267799f,0.939566f,0.078679f},{0.196013f,0.851596f,0.443852f},{-0.260303f,1.017915f,0.105272f},{-0.062051f,1.369231f,0.195527f},{-0.531478f,-0.917087f,0.276049f},{-0.06249f,-0.766591f,-0.65404f},{-0.258348f,-0.695167f,-0.792367f},{-0.329108f,1.401578f,0.235152f},{-0.288265f,0.967705f,0.279879f},{0.226703f,-0.07935f,0.025564f},{-0.148809f,1.378121f,0.320027f},{-0.142245f,1.383152f,0.301441f},{-0.1429f,1.399398f,0.335318f},{-0.131582f,1.425068f,0.280731f},{-0.136056f,1.393858f,0.276614f},{-0.130027f,1.393428f,0.252069f},{-0.115956f,1.424568f,0.22164f},{-0.124239f,1.38877f,0.23067f},{-0.195184f,1.392851f,0.203788f},{-0.206274f,1.412491f,0.187417f},{-0.241488f,1.39454f,0.230161f},{-0.264454f,1.400769f,0.221223f},{-0.25791f,1.403741f,0.247871f},{-0.287269f,1.406883f,0.244429f},{-0.262065f,1.395744f,0.284528f},{-0.289936f,1.393584f,0.290848f},{-0.211786f,1.399492f,0.34039f},{-0.201399f,1.389485f,0.323492f},{-0.280714f,1.492141f,0.347962f},{-0.285765f,1.518747f,0.308234f},{-0.277276f,1.530338f,0.262555f},{-0.333461f,1.488034f,0.233088f},{-0.366041f,1.453056f,0.257133f},{-0.3829f,1.445471f,0.27533f},{-0.377518f,1.431347f,0.310069f},{-0.326315f,1.463132f,0.350616f},{-0.345178f,1.539286f,0.364174f},{-0.359718f,1.566742f,0.324933f},{-0.356043f,1.584352f,0.278446f},{-0.398045f,1.526292f,0.244304f},{-0.418166f,1.479291f,0.266408f},{-0.431948f,1.46489f,0.284624f},{-0.421305f,1.450197f,0.319641f},{-0.380528f,1.495432f,0.364453f},{-0.490753f,1.570393f,0.395227f},{-0.525253f,1.579776f,0.361834f},{-0.542778f,1.600036f,0.321136f},{-0.522765f,1.541824f,0.273906f},{-0.493311f,1.497233f,0.284314f},{-0.487103f,1.47541f,0.297799f},{-0.465476f,1.470253f,0.329289f},{-0.474605f,1.518937f,0.384142f},{-0.589262f,1.56409f,0.407902f},{-0.619013f,1.569656f,0.380032f},{-0.635502f,1.585899f,0.346218f},{-0.61395f,1.539806f,0.305366f},{-0.578421f,1.486814f,0.324144f},{-0.559803f,1.483728f,0.350506f},{-0.57142f,1.522628f,0.397575f},{-0.698381f,1.544691f,0.426377f},{-0.72079f,1.556779f,0.401163f},{-0.729047f,1.572664f,0.370292f},{-0.727075f,1.525484f,0.338674f},{-0.701383f,1.466133f,0.383078f},{-0.715607f,1.472703f,0.35959f},{-0.697643f,1.504754f,0.420825f},{-0.875124f,1.489145f,0.45454f},{-0.907053f,1.496081f,0.417258f},{-0.886304f,1.474514f,0.390155f},{-0.84361f,1.449492f,0.415802f},{-0.85544f,1.448989f,0.39961f},{-0.988473f,1.436911f,0.435471f},{-1.009751f,1.402303f,0.415395f},{-1.019388f,1.357156f,0.45761f},{-1.026166f,1.365391f,0.437783f},{-1.064602f,1.37547f,0.467655f},{-0.980105f,1.410878f,0.483926f} };
		csgjs_vector normals[] = { {-0.206657f,0.443353f,-0.872199f},{-0.494962f,0.293368f,-0.817892f},{-0.381406f,0.520847f,-0.763707f},{-0.892182f,0.162939f,-0.421262f},{-0.763333f,0.378426f,-0.523561f},{-0.964487f,-0.002145f,0.264122f},{-0.840484f,0.289296f,0.458142f},{-0.988058f,-0.082861f,-0.129904f},{-0.859069f,0.449945f,0.244029f},{-0.729086f,-0.066846f,0.68115f},{-0.64847f,0.149768f,0.746362f},{-0.536232f,0.042195f,0.843015f},{-0.02501f,0.676697f,-0.735837f},{-0.245955f,0.48212f,0.840872f},{-0.03811f,0.818637f,0.573045f},{-0.358296f,0.252f,0.898955f},{0.927408f,-0.221783f,-0.30121f},{0.794382f,-0.484782f,-0.365984f},{0.676791f,-0.255323f,-0.690481f},{0.741163f,-0.018346f,-0.671074f},{0.922223f,-0.375881f,0.09065f},{0.775616f,-0.631205f,0.000163f},{0.774542f,-0.412555f,0.479461f},{0.649332f,-0.664009f,0.370755f},{0.437454f,-0.378361f,0.815768f},{0.40509f,-0.596339f,0.693024f},{0.578429f,-0.64558f,-0.498645f},{0.439696f,-0.390693f,-0.808719f},{0.595412f,-0.799863f,-0.075523f},{0.414464f,-0.846858f,0.333245f},{0.155288f,-0.74507f,0.648657f},{0.264335f,-0.768607f,-0.582555f},{0.18746f,-0.467422f,-0.86393f},{0.321744f,-0.932994f,-0.161252f},{0.103655f,-0.965992f,0.236884f},{-0.093931f,-0.81491f,0.571926f},{0.23032f,0.219442f,-0.948049f},{0.388353f,-0.024522f,-0.921184f},{0.186784f,-0.125893f,-0.974301f},{0.06933f,-0.409026f,-0.909885f},{-0.011838f,-0.749566f,-0.661824f},{0.015699f,-0.972806f,-0.231087f},{-0.18315f,-0.960064f,0.2115f},{-0.225451f,-0.772157f,0.594092f},{0.02442f,-0.457972f,0.888631f},{-0.160532f,-0.551055f,0.818882f},{-0.19019f,-0.212374f,0.958502f},{0.956339f,-0.259871f,0.133726f},{0.779392f,-0.289774f,0.555499f},{0.326998f,-0.341286f,0.881247f},{0.0362f,-0.013277f,-0.999256f},{-0.073879f,-0.287767f,-0.954847f},{-0.210512f,-0.599332f,-0.772325f},{-0.388743f,-0.87544f,-0.287199f},{-0.420355f,-0.857138f,0.297684f},{-0.384285f,-0.671586f,0.63348f},{-0.322669f,-0.455876f,0.829495f},{-0.420008f,0.022731f,-0.907236f},{-0.517114f,0.376776f,-0.768526f},{-0.130934f,0.257027f,-0.957493f},{-0.75453f,-0.038571f,-0.655131f},{-0.794411f,0.35058f,-0.495989f},{-0.967663f,-0.12334f,-0.220036f},{-0.950142f,0.288872f,-0.117406f},{-0.932865f,-0.257126f,0.252287f},{-0.94641f,0.164286f,0.278063f},{-0.716294f,-0.342295f,0.608077f},{-0.785771f,0.044042f,0.616948f},{-0.492973f,-0.176964f,0.851858f},{-0.365514f,0.505205f,-0.781772f},{-0.682563f,0.535201f,-0.497663f},{-0.894714f,0.441173f,-0.069663f},{-0.857246f,0.319445f,0.403837f},{-0.656381f,0.140494f,0.741232f},{-0.604187f,-0.346267f,-0.717675f},{-0.279938f,-0.160177f,-0.946561f},{-0.87303f,-0.406414f,-0.269529f},{-0.788994f,-0.572675f,0.222554f},{-0.585151f,-0.53578f,0.608718f},{-0.195618f,-0.3162f,-0.928305f},{-0.057774f,-0.022865f,-0.998068f},{-0.435141f,-0.578697f,-0.689755f},{-0.639703f,-0.709134f,-0.296495f},{-0.601774f,-0.782916f,0.157831f},{-0.4867f,-0.674275f,0.555406f},{-0.410261f,-0.458732f,0.788195f},{-0.527518f,0.765006f,-0.369446f},{-0.291431f,0.687821f,-0.664809f},{-0.730382f,0.682684f,0.022026f},{-0.685737f,0.566658f,0.456797f},{-0.569089f,0.347266f,0.745348f},{-0.07774f,0.453036f,-0.888096f},{0.075999f,0.44703f,-0.891285f},{-0.189271f,0.663456f,-0.72388f},{-0.420406f,0.835952f,-0.352764f},{-0.597446f,0.798277f,0.076236f},{-0.587549f,0.629484f,0.508465f},{-0.502321f,0.314094f,0.805617f},{-0.436608f,0.043469f,0.898601f},{-0.356928f,0.104324f,0.928288f},{0.092359f,0.72377f,-0.683832f},{-0.062337f,0.535f,-0.842549f},{-0.031007f,0.589756f,0.806986f},{-0.222326f,0.328519f,0.917958f},{-0.409594f,0.120675f,0.904251f},{-0.277038f,0.32636f,-0.903736f},{-0.517669f,0.126616f,-0.84616f},{-0.862132f,-0.122296f,-0.491703f},{-0.608446f,-0.079896f,0.789563f},{-0.837978f,-0.248936f,0.485617f},{0.1339f,0.577449f,-0.805371f},{0.363775f,0.531862f,-0.764716f},{0.074513f,0.401758f,-0.912709f},{-0.134291f,0.405387f,-0.904227f},{0.370022f,0.699363f,-0.611535f},{0.58526f,0.619887f,-0.522696f},{0.447019f,0.876084f,-0.180697f},{0.758997f,0.634424f,-0.146385f},{0.428668f,0.8555f,0.290453f},{0.817397f,0.50284f,0.281096f},{0.257342f,0.730484f,0.632588f},{0.593328f,0.44622f,0.669963f},{0.079342f,0.350894f,0.933048f},{0.300803f,0.283404f,0.910604f},{-0.216132f,0.153925f,0.964154f},{0.014986f,0.105803f,0.994274f},{-0.486271f,-0.023287f,0.873498f},{-0.294592f,-0.049627f,0.954334f},{-0.235991f,0.238752f,-0.941969f},{-0.374925f,0.219934f,-0.900589f},{-0.506106f,0.048758f,-0.861092f},{-0.6049f,0.024494f,-0.795925f},{-0.578567f,-0.180739f,0.795357f},{-0.688903f,-0.168385f,0.705024f},{-0.768579f,-0.278949f,0.575737f},{-0.894689f,-0.259441f,0.363623f},{0.501562f,0.467581f,-0.727876f},{0.230667f,0.364104f,-0.902342f},{0.749423f,0.541008f,-0.381674f},{0.513846f,0.247439f,0.821423f},{0.163636f,0.051684f,0.985166f},{-0.141545f,-0.092134f,0.985635f},{-0.100692f,0.24785f,-0.963552f},{-0.436242f,0.136832f,-0.889365f},{-0.460598f,-0.193154f,0.866338f},{-0.720029f,-0.230277f,0.654623f},{0.506492f,0.247695f,-0.825901f},{0.452314f,0.226838f,-0.862529f},{0.177691f,0.248287f,-0.95225f},{0.16545f,0.24663f,-0.954882f},{0.949785f,0.205157f,0.23626f},{0.977812f,-0.135933f,0.159394f},{0.9483f,0.298829f,-0.106902f},{0.817618f,0.137229f,0.559168f},{-0.198047f,-0.211564f,0.957088f},{-0.21027f,-0.212908f,0.954178f},{0.091682f,-0.214898f,0.972324f},{0.153131f,-0.185261f,0.970685f},{-0.146714f,0.285499f,-0.947083f},{-0.176546f,0.208127f,-0.962037f},{-0.425829f,0.32611f,-0.843992f},{-0.501352f,0.1513f,-0.851912f},{-0.78404f,0.141074f,-0.604466f},{-0.728183f,-0.041075f,0.684151f},{-0.773487f,-0.200258f,0.601344f},{-0.956267f,-0.076092f,0.282424f},{-0.501645f,-0.147333f,0.852435f},{-0.527441f,-0.221132f,0.820309f},{0.424888f,0.21346f,-0.879718f},{0.547234f,0.173764f,-0.818744f},{0.252771f,0.326821f,-0.910656f},{0.108414f,0.331326f,-0.937267f},{0.844498f,0.061532f,-0.532012f},{0.061182f,-0.232122f,0.970761f},{0.200163f,-0.251263f,0.946996f},{0.588985f,-0.251455f,0.768028f},{-0.263902f,-0.124496f,0.956481f},{-0.119735f,-0.12957f,0.984314f},{-0.089007f,0.476578f,-0.874615f},{-0.228524f,0.39459f,-0.889987f},{-0.399463f,0.577682f,-0.711838f},{-0.53084f,0.376161f,-0.759416f},{-0.437464f,0.049578f,0.897868f},{-0.566221f,-0.016993f,0.824078f},{-0.676342f,0.239247f,0.696651f},{-0.798572f,0.050672f,0.599762f},{0.511562f,0.351253f,-0.784172f},{0.638231f,0.27156f,-0.720359f},{0.392837f,0.460784f,-0.795837f},{0.216265f,0.551676f,-0.805533f},{0.822634f,0.093712f,-0.560796f},{0.87414f,0.082657f,-0.478589f},{0.163858f,-0.077482f,0.983436f},{0.308924f,-0.133397f,0.941685f},{0.55401f,-0.240576f,0.796992f},{0.630699f,-0.218376f,0.744668f},{-0.135197f,0.123008f,0.983154f},{0.040038f,0.029456f,0.998764f},{0.106375f,0.652764f,-0.750055f},{-0.064403f,0.702421f,-0.708842f},{-0.148917f,0.791872f,-0.592252f},{-0.278647f,0.793031f,-0.541717f},{-0.225857f,0.252224f,0.940942f},{-0.372161f,0.326165f,0.868972f},{-0.430155f,0.465653f,0.773391f},{-0.518724f,0.497992f,0.694931f},{0.524185f,0.334469f,-0.783174f},{0.505266f,0.234389f,-0.830523f},{0.290621f,0.354602f,-0.888705f},{0.255629f,0.523359f,-0.812865f},{0.740257f,0.185053f,-0.646355f},{0.773089f,0.121109f,-0.622628f},{0.950976f,-0.017475f,-0.30877f},{0.94808f,-0.08326f,-0.306939f},{0.965908f,-0.200042f,0.16433f},{0.460462f,-0.157157f,0.873657f},{0.56426f,-0.223352f,0.794811f},{0.779409f,-0.230919f,0.582407f},{0.188692f,-0.078524f,0.978892f},{0.264384f,-0.18031f,0.947412f},{-0.093379f,0.093056f,0.991272f},{-0.055624f,-0.106292f,0.992778f},{0.017403f,0.580316f,-0.814205f},{-0.008197f,0.711527f,-0.702611f},{-0.285173f,0.046649f,0.95734f},{-0.333991f,0.316119f,0.887986f},{-0.224638f,0.793212f,-0.565997f},{-0.479854f,0.423094f,0.76859f},{0.058481f,0.587904f,-0.806814f},{0.355347f,0.410145f,-0.839946f},{-0.219015f,0.713674f,-0.665358f},{-0.402892f,0.857098f,-0.32103f},{-0.454159f,0.780967f,-0.428755f},{-0.554045f,0.827926f,0.087023f},{-0.639874f,0.766316f,0.057632f},{-0.564155f,0.653253f,0.504965f},{-0.642374f,0.550862f,0.532829f},{-0.502426f,0.36515f,0.783731f},{-0.285345f,0.166081f,0.943925f},{-0.006336f,-0.032843f,0.99944f},{0.592803f,0.218366f,-0.775178f},{0.893929f,-0.063408f,-0.4437f},{0.254431f,-0.196815f,0.946852f},{0.677354f,-0.328605f,0.658187f},{-0.876135f,0.478681f,-0.05703f},{-0.980734f,0.093117f,-0.171725f},{-0.737274f,0.509563f,-0.44359f},{-0.888161f,0.325549f,0.324327f},{-0.974583f,-0.09753f,-0.201682f},{0.877353f,0.139031f,-0.459263f},{0.638406f,-0.153798f,0.754178f},{-0.771284f,-0.00095f,-0.63649f},{0.191963f,0.876163f,-0.442141f},{0.055852f,-0.286198f,0.956541f},{0.676833f,0.038562f,-0.735126f},{0.477555f,0.14426f,-0.866678f},{-0.896047f,0.380152f,-0.229314f},{-0.924809f,-0.212623f,0.315466f},{0.969761f,-0.194857f,0.14695f},{0.275368f,0.802501f,0.529306f},{0.285669f,0.858668f,0.425538f},{0.507636f,0.344962f,0.789498f},{0.636042f,0.697741f,0.329556f},{0.310425f,0.939708f,0.143478f},{0.731208f,0.673175f,0.110319f},{0.55338f,0.627575f,-0.547649f},{0.346803f,0.845156f,-0.406742f},{0.06869f,0.720478f,-0.690067f},{0.01705f,0.324395f,-0.945768f},{-0.294731f,0.498862f,-0.815028f},{-0.445712f,-0.450465f,-0.773578f},{-0.063026f,0.947044f,-0.314857f},{-0.365858f,-0.871783f,-0.325794f},{-0.13745f,0.930217f,0.340301f},{-0.31572f,-0.895481f,0.313743f},{-0.106419f,-0.500853f,0.858966f},{-0.177212f,0.530696f,0.828829f},{0.409334f,0.460502f,0.787645f},{0.554716f,0.755355f,0.348897f},{0.376712f,0.801494f,-0.464429f},{-0.135506f,0.217533f,-0.966601f},{-0.413576f,-0.39413f,-0.820741f},{-0.436225f,-0.809691f,-0.392567f},{-0.256596f,-0.813956f,0.521185f},{-0.101601f,-0.479187f,0.871812f},{0.344432f,0.312703f,0.885202f},{0.265643f,0.897086f,0.353087f},{0.124611f,0.816441f,-0.563823f},{-0.189976f,0.086927f,-0.977933f},{-0.327767f,-0.418447f,-0.847037f},{-0.356645f,-0.821207f,-0.445448f},{-0.299551f,-0.879713f,0.369288f},{-0.022848f,-0.533805f,0.845299f},{0.198528f,0.526111f,0.826918f},{0.065615f,0.948094f,0.311148f},{-0.069153f,0.96433f,-0.25551f},{-0.238602f,0.249662f,-0.938476f},{-0.29836f,-0.431825f,-0.85118f},{-0.21751f,-0.929631f,-0.297448f},{-0.10149f,-0.866479f,0.488789f},{0.090413f,-0.498208f,0.862331f},{0.097587f,0.426854f,0.89904f},{-0.038551f,0.916939f,0.39716f},{-0.241249f,0.883006f,-0.402616f},{-0.292335f,0.039448f,-0.955502f},{-0.185178f,-0.702293f,-0.687382f},{0.074353f,-0.895876f,0.438038f},{0.184642f,-0.52094f,0.833384f},{0.035052f,0.500766f,0.864873f},{-0.154455f,0.872613f,0.463347f},{-0.393735f,0.787762f,-0.473713f},{-0.310906f,0.145845f,-0.939184f},{0.194306f,-0.8935f,0.40485f},{-0.127207f,-0.613003f,-0.779773f},{0.218501f,-0.459997f,0.860616f},{-0.02094f,0.324837f,0.945538f},{-0.434756f,0.900078f,0.029106f},{-0.301221f,0.04514f,-0.952485f},{0.360589f,-0.745331f,0.560766f},{0.108746f,-0.718144f,-0.687345f},{-0.57094f,0.819281f,0.05298f},{-0.493662f,0.2331f,-0.837832f},{0.391238f,-0.864994f,0.314193f},{0.004032f,-0.582702f,-0.812676f},{-0.834686f,0.437618f,0.33435f},{0.1721f,-0.077835f,0.982f} };
		struct Triangle {
			int a, b, c;
		}triangles[] = { {0,1,2},{3,4,2},{3,2,1},{5,6,7},{6,8,7},{9,10,5},{10,6,5},{10,9,11},{0,2,12},{6,13,14},{6,14,8},{10,15,13},{10,13,6},{15,10,11},{16,17,18},{16,18,19},{20,21,17},{20,17,16},{22,23,20},{23,21,20},{24,25,22},{25,23,22},{17,26,18},{26,27,18},{21,28,17},{28,26,17},{23,29,28},{23,28,21},{25,30,29},{25,29,23},{26,31,27},{31,32,27},{28,33,26},{33,31,26},{29,34,33},{29,33,28},{30,35,34},{30,34,29},{36,37,38},{32,39,37},{39,38,37},{31,40,32},{40,39,32},{33,41,31},{41,40,31},{34,42,41},{34,41,33},{35,43,42},{35,42,34},{44,45,43},{44,43,35},{45,44,46},{47,20,16},{48,22,47},{22,20,47},{49,24,48},{24,22,48},{36,38,50},{39,51,38},{51,50,38},{40,52,39},{52,51,39},{41,53,40},{53,52,40},{42,54,53},{42,53,41},{43,55,54},{43,54,42},{45,56,55},{45,55,43},{56,45,46},{57,58,59},{60,61,58},{60,58,57},{62,63,61},{62,61,60},{64,65,62},{65,63,62},{66,67,64},{67,65,64},{68,67,66},{58,69,59},{61,70,58},{70,69,58},{63,71,61},{71,70,61},{65,72,71},{65,71,63},{67,73,72},{67,72,65},{68,73,67},{74,60,57},{74,57,75},{76,62,60},{76,60,74},{77,64,76},{64,62,76},{78,66,77},{66,64,77},{79,75,80},{81,74,75},{81,75,79},{82,76,74},{82,74,81},{83,77,82},{77,76,82},{84,78,83},{78,77,83},{85,78,84},{52,81,79},{52,79,51},{53,82,81},{53,81,52},{54,83,53},{83,82,53},{55,84,54},{84,83,54},{70,86,69},{86,87,69},{71,88,70},{88,86,70},{72,89,88},{72,88,71},{73,90,89},{73,89,72},{36,91,92},{87,93,91},{93,92,91},{86,94,87},{94,93,87},{88,95,86},{95,94,86},{89,96,95},{89,95,88},{90,97,96},{90,96,89},{98,99,97},{98,97,90},{99,98,46},{12,100,101},{12,101,0},{13,102,14},{15,103,13},{103,102,13},{11,104,15},{104,103,15},{105,1,0},{105,0,101},{106,3,1},{106,1,105},{107,3,106},{108,9,109},{9,5,109},{104,11,108},{11,9,108},{110,111,112},{110,112,113},{114,115,111},{114,111,110},{116,117,115},{116,115,114},{118,119,117},{118,117,116},{120,121,118},{121,119,118},{122,123,120},{123,121,120},{124,125,122},{125,123,122},{126,127,124},{127,125,124},{128,129,113},{128,113,112},{130,131,129},{130,129,128},{132,133,134},{133,135,134},{127,126,132},{126,133,132},{111,136,137},{111,137,112},{115,138,136},{115,136,111},{117,138,115},{123,139,121},{125,140,123},{140,139,123},{127,141,125},{141,140,125},{142,128,112},{142,112,137},{143,130,128},{143,128,142},{144,132,145},{132,134,145},{141,127,144},{127,132,144},{146,147,148},{147,149,148},{150,151,152},{153,151,150},{154,155,156},{154,156,157},{158,159,149},{159,148,149},{160,161,158},{161,159,158},{162,161,160},{163,164,165},{166,167,164},{166,164,163},{155,154,167},{155,167,166},{168,169,170},{168,170,171},{172,169,168},{173,174,175},{176,177,173},{177,174,173},{178,179,171},{178,171,170},{180,181,179},{180,179,178},{182,183,184},{183,185,184},{177,176,182},{176,183,182},{186,187,188},{186,188,189},{190,191,187},{190,187,186},{192,193,194},{193,195,194},{196,197,192},{197,193,192},{198,199,189},{198,189,188},{200,201,199},{200,199,198},{202,203,204},{203,205,204},{197,196,202},{196,203,202},{206,207,208},{206,208,209},{210,211,207},{210,207,206},{212,213,211},{212,211,210},{214,47,213},{214,213,212},{215,216,217},{218,219,215},{219,216,215},{220,221,218},{221,219,218},{222,223,209},{222,209,208},{221,220,224},{220,225,224},{223,198,209},{198,188,209},{226,200,223},{200,198,223},{225,202,204},{225,204,227},{220,197,202},{220,202,225},{187,206,188},{206,209,188},{191,210,187},{210,206,187},{212,210,191},{195,215,217},{193,218,215},{193,215,195},{197,220,218},{197,218,193},{199,228,189},{228,229,189},{201,230,199},{230,228,199},{231,232,201},{232,230,201},{233,234,231},{234,232,231},{235,236,234},{235,234,233},{205,237,236},{205,236,235},{203,238,237},{203,237,205},{196,239,238},{196,238,203},{240,186,229},{186,189,229},{241,190,240},{190,186,240},{242,192,194},{242,194,243},{239,196,192},{239,192,242},{228,178,170},{228,170,229},{230,180,178},{230,178,228},{232,180,230},{237,184,236},{238,182,237},{182,184,237},{239,177,238},{177,182,238},{169,240,229},{169,229,170},{177,239,174},{239,242,174},{179,158,171},{158,149,171},{181,160,179},{160,158,179},{244,245,246},{247,245,244},{183,166,163},{183,163,185},{176,155,166},{176,166,183},{147,168,149},{168,171,149},{172,168,147},{156,173,175},{155,176,173},{155,173,156},{159,142,148},{142,137,148},{161,143,159},{143,142,159},{245,248,162},{165,248,245},{167,144,145},{167,145,164},{154,141,144},{154,144,167},{136,146,137},{146,148,137},{138,249,136},{249,146,136},{152,249,138},{139,250,153},{140,157,250},{140,250,139},{141,154,157},{141,157,140},{129,105,113},{105,101,113},{131,106,129},{106,105,129},{251,107,131},{107,106,131},{133,108,109},{133,109,135},{126,104,108},{126,108,133},{100,110,101},{110,113,101},{252,114,100},{114,110,100},{116,114,252},{102,122,120},{103,124,122},{103,122,102},{104,126,124},{104,124,103},{46,221,224},{46,224,99},{222,92,93},{208,36,92},{208,92,222},{216,49,48},{219,253,49},{219,49,216},{221,46,253},{221,253,219},{254,211,213},{255,207,211},{255,211,254},{36,208,207},{36,207,255},{8,256,7},{7,256,4},{107,7,3},{7,107,251},{248,7,251},{257,7,248},{7,4,3},{36,255,37},{18,27,37},{27,32,37},{19,18,37},{254,19,37},{254,37,255},{44,30,25},{44,35,30},{44,253,46},{44,24,49},{44,25,24},{253,44,49},{36,80,59},{75,57,59},{75,59,80},{68,66,78},{68,85,46},{85,68,78},{36,50,80},{51,79,80},{51,80,50},{85,84,55},{85,56,46},{56,85,55},{36,59,91},{69,91,59},{69,87,91},{68,98,73},{98,68,46},{98,90,73},{109,5,7},{135,109,7},{135,7,257},{251,131,130},{134,135,257},{162,160,181},{245,162,246},{246,162,181},{247,165,245},{185,163,165},{185,165,247},{246,181,180},{232,246,180},{234,244,246},{234,246,232},{247,244,234},{184,185,247},{184,247,236},{236,247,234},{231,201,200},{231,200,226},{204,205,235},{227,204,235},{226,223,222},{222,93,226},{226,93,94},{224,225,227},{99,224,227},{99,227,97},{258,214,212},{217,214,258},{172,241,169},{241,240,169},{174,243,175},{174,242,243},{249,172,146},{172,147,146},{157,175,250},{157,156,175},{117,152,138},{119,152,117},{119,150,152},{121,153,119},{139,153,121},{153,150,119},{102,120,14},{217,48,214},{216,48,217},{48,47,214},{213,16,19},{47,16,213},{213,19,254},{251,130,143},{251,143,161},{248,251,162},{162,251,161},{165,257,248},{145,134,257},{164,145,257},{164,257,165},{94,231,226},{95,233,94},{233,231,94},{96,233,95},{97,227,96},{227,235,96},{96,235,233},{259,260,261},{262,261,260},{260,263,262},{263,264,262},{265,262,264},{264,266,265},{266,267,265},{268,265,267},{267,269,268},{270,268,269},{269,271,270},{272,270,271},{271,273,272},{274,272,273},{275,274,273},{273,276,275},{276,259,275},{261,275,259},{261,262,277},{278,277,262},{262,265,279},{279,278,262},{265,268,279},{280,279,268},{268,270,280},{281,280,270},{270,272,281},{282,281,272},{272,274,282},{283,282,274},{274,275,284},{284,283,274},{275,261,277},{277,284,275},{277,278,285},{286,285,278},{278,279,287},{287,286,278},{279,280,287},{288,287,280},{280,281,288},{289,288,281},{281,282,289},{290,289,282},{282,283,291},{291,290,282},{283,284,292},{292,291,283},{284,277,285},{285,292,284},{285,286,293},{294,293,286},{286,287,295},{295,294,286},{287,288,296},{296,295,287},{288,289,297},{297,296,288},{289,290,297},{298,297,290},{290,291,299},{299,298,290},{291,292,299},{300,299,292},{292,285,300},{293,300,285},{293,294,301},{302,301,294},{294,295,302},{303,302,295},{295,296,304},{304,303,295},{297,298,305},{298,299,306},{306,305,298},{299,300,306},{307,306,300},{300,293,307},{301,307,293},{301,302,308},{309,308,302},{302,303,309},{310,309,303},{303,304,311},{311,310,303},{305,306,312},{312,313,305},{306,307,312},{314,312,307},{307,301,314},{308,314,301},{308,309,315},{309,310,316},{310,311,317},{317,316,310},{313,312,318},{318,319,313},{312,314,318},{316,317,320},{321,320,317},{319,318,322},{322,323,319},{320,321,324},{323,322,324},{118,263,260},{260,120,118},{116,264,263},{263,118,116},{116,252,266},{264,116,266},{4,256,271},{269,4,271},{256,8,273},{271,256,273},{14,276,273},{273,8,14},{276,14,259},{120,260,259},{14,120,259},{258,190,241},{258,191,190},{258,212,191},{151,241,172},{151,172,249},{151,258,241},{152,151,249},{175,243,151},{250,175,151},{250,151,153},{243,258,151},{243,194,258},{195,217,258},{194,195,258},{4,267,2},{267,4,269},{267,12,2},{267,100,12},{267,252,100},{266,252,267},{296,297,305},{305,304,296},{304,305,313},{313,311,304},{316,315,309},{315,316,320},{311,313,317},{319,317,313},{315,318,314},{314,308,315},{320,325,315},{325,320,324},{323,321,317},{321,323,324},{317,319,323},{318,315,325},{325,322,318},{322,325,324}};
        // clang-format on

        std::vector<csgjs_polygon> polygons;
        for (const auto &tri : triangles) {
            std::vector<csgjs_vertex> verts;

            verts.push_back({vertices[tri.a], normals[tri.a], green});
            verts.push_back({vertices[tri.b], normals[tri.b], green});
            verts.push_back({vertices[tri.c], normals[tri.c], green});

            polygons.push_back(csgjs_polygon(verts));
        }

        auto gourd = csgjs_modelFromPolygons(polygons);
        auto cyl = csgsmodel_cylinder({0.6f, 0.8f, -0.6f}, {-0.6f, -0.8f, 0.6f}, 0.4f, blue);

        {
            exunit::Timer t;
            exunit::modeltoply("gourd_union.ply", csgjs_union(gourd, cyl));
            std::cout << "gourd union cyl " << t.GetElapsedMS() << "ms" << '\n';
        }
        {
            exunit::Timer t;
            exunit::modeltoply("gourd_intersect.ply", csgjs_intersection(gourd, cyl));
            std::cout << "gourd intersect cyl " << t.GetElapsedMS() << "ms" << '\n';
        }

        {
            exunit::Timer t;
            exunit::modeltoply("gourd_subtract.ply", csgjs_subtract(gourd, cyl));
            std::cout << "gourd subtract cyl " << t.GetElapsedMS() << "ms" << '\n';
        }
        {
            exunit::Timer t;
            exunit::modeltoply("cylinder_subtract_gourd.ply", csgjs_subtract(cyl, gourd));
            std::cout << "cyl subtract gourd " << t.GetElapsedMS() << "ms" << '\n';
        }
    }

    return 0;
}