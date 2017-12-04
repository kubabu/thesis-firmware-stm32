/*
 * knn_tests.c
 *
 *  Created on: Oct 30, 2017
 *      Author: kuba
 */
#include "classifiers.h"
#include "tests.h"


int circle_ccw_test(void) {
    const float circle_ccw_test_sample[FEATURES][DTW_SEQUENCE_LEN] = {
		{-0.0647563f,  0.0106699f,  0.0106699f, -0.0647563f, -0.0647563f, -0.0270432f,  -0.140182f,  -0.215609f,  -0.253322f, -0.0270432f,   0.086096f,   0.350087f,   0.538653f,   0.802644f,   0.840357f,   0.915784f,    0.99121f,    0.99121f,   0.840357f,   0.802644f,   0.764931f,   0.350087f,   0.161522f,  0.0483829f,  -0.102469f,  0.0106699f, -0.0647563f,  -0.102469f,  -0.102469f,  -0.102469f, -0.0270432f,  0.0483829f,  0.0483829f,   0.086096f,   0.123809f,   0.161522f,   0.123809f,   0.161522f,   0.161522f,   0.161522f,   0.199235f,   0.161522f,   0.161522f,   0.161522f,   0.161522f,   0.161522f,   0.161522f,   0.161522f,   0.161522f,   0.086096f,   0.123809f,   0.123809f,   0.123809f,   0.123809f},
		{ -0.171663f,  -0.108154f,  -0.171663f, -0.0446449f,  -0.203417f,  -0.203417f,  0.0506187f,  0.0823732f,    1.35255f,    1.25729f,    1.13027f,   0.971499f,   0.431673f,  0.0506187f,  -0.203417f,  -0.838508f,    -1.1243f,  -0.870262f,  -0.870262f,  0.0823732f,     0.2729f,  0.0188642f,  -0.330435f,  -0.552717f,  -0.552717f,  -0.425699f,   -0.36219f,  -0.425699f,  -0.266926f,  -0.203417f,  -0.235172f,  -0.108154f, -0.0128904f,  -0.171663f, -0.0763994f,  -0.235172f,  -0.235172f,  -0.330435f,  -0.235172f,  -0.108154f,  -0.235172f,  -0.171663f,  -0.235172f,  -0.235172f,  -0.235172f,  -0.235172f,  -0.203417f,  -0.203417f,  -0.266926f,  -0.139908f,  -0.139908f,  -0.108154f,  -0.171663f,  -0.203417f},
		{  0.228625f,   0.228625f,   0.380845f,   0.441733f,   0.533065f,   0.533065f,   0.624397f,    1.17239f,   0.989725f,   0.654841f,   0.167737f,  -0.410699f,  -0.776026f,  -0.715138f,  -0.532475f,  -0.380255f,  -0.106259f,   0.259069f,   0.167737f,   0.198181f,   0.380845f,   0.593953f,   0.715729f,   0.715729f,   0.928837f,   0.807061f,   0.624397f,   0.563509f,   0.441733f,   0.441733f,   0.472177f,   0.380845f,   0.289513f,   0.319957f,   0.319957f,   0.319957f,   0.380845f,   0.380845f,   0.380845f,   0.350401f,   0.411289f,   0.411289f,   0.441733f,   0.350401f,   0.350401f,   0.380845f,   0.380845f,   0.411289f,   0.472177f,   0.411289f,   0.350401f,   0.380845f,   0.350401f,   0.411289f},
		{-0.0437324f,   0.210893f,    0.36187f,   0.484889f,   0.298563f,   0.278992f,   0.560378f,    1.24836f,   0.514046f,  -0.143585f, -0.0449306f, -0.0631039f, -0.0327486f,  -0.151973f,  0.0195743f,  0.0281617f,   0.330117f,   0.587138f,   0.876911f, -0.0764842f,  -0.301752f,  -0.204496f,  -0.301752f,  -0.232454f,  0.0171779f, -0.0155739f, -0.0788806f, -0.0327486f, -0.0375415f,   0.248637f,   0.053724f,   0.130411f,   0.262017f,   0.202305f,   0.187727f,   0.216884f,   0.281389f,   0.203503f,   0.163363f,  0.0513276f,   0.147586f,   0.133007f,    0.14499f,  0.0732952f,   0.135404f,    0.12442f,  0.0465346f,   0.227868f,   0.106047f,  0.0561205f,   0.157172f,    0.12442f,   0.160966f,   0.125618f},
		{  0.072118f,   0.156032f,   0.171831f,   0.140233f,  0.0563194f, -0.0400263f,  -0.095192f,   -0.40469f,   -1.10915f,   -1.47382f,   -1.60331f,    -1.2876f,  -0.802763f,  -0.317927f,  0.0894706f,   0.361155f,   0.756121f,   0.933013f,   0.907632f,   0.820869f,   0.770365f,   0.766998f,   0.642422f,   0.550738f,   0.290191f, -0.00350818f, -0.0871632f,  -0.178847f,  -0.202674f,  -0.144142f,  -0.238934f,  -0.230905f,  -0.156832f,  -0.134559f, -0.0967459f, -0.0620408f, -0.0177528f, -0.0335515f, -0.0146449f, -0.0462421f, -0.00506214f, -0.0319975f, -0.0257816f, -0.0604868f, -0.0066161f, -0.00998302f, -0.0177528f,  0.0389668f, -0.0288896f, -0.0589328f, -0.0366594f, -0.0288896f,  -0.011537f, -0.00195422f},
		{   0.18323f, -0.0427586f, -0.0875492f,   -0.23658f,  -0.271191f,  -0.447503f,  -0.685708f,  -0.822523f,  -0.668606f,   0.354249f,    1.28793f,     2.0848f,    2.62391f,    2.72815f,    2.68825f,    2.24401f,    1.43207f,    0.46134f,  -0.782618f,  -0.668606f,  -0.169394f,  0.0789905f,   0.195446f, -0.0700401f,  -0.680821f,     -1.031f,  -0.857134f,   -0.86202f,  -0.872199f,  -0.807457f,  -0.695887f,  -0.583911f,  -0.355479f,  -0.109945f, -0.0802198f,  0.0488586f,    0.13111f,   0.116045f,  0.0614814f,  0.0565952f,  0.0814336f,  0.0488586f,   0.123374f,   0.113601f,  0.0936492f,   0.021577f,   0.054152f,  0.0142477f,  0.0439724f,  0.0341999f,  0.0639245f,  0.0985355f,   0.108715f,   0.160835f},
		{ -0.167566f,   -0.11091f,  -0.167566f, -0.0572623f,  -0.223721f,  -0.217203f, 0.00992277f, -0.0111352f,    0.90539f,   0.955026f,    1.03525f,    1.13552f,   0.668236f,   0.121229f,  -0.209682f,  -0.884541f,   -1.09562f,  -0.807329f,  -0.815852f,  0.0455208f,   0.206966f, -0.0347002f,  -0.328008f,  -0.503492f,  -0.494968f,  -0.398703f,  -0.347061f,   -0.40121f,  -0.274862f,  -0.210685f,  -0.232244f,  -0.134475f, -0.0276808f,  -0.167065f, -0.0878466f,  -0.247286f,  -0.231242f,   -0.32149f,  -0.241269f,   -0.13247f,  -0.228735f,  -0.177594f,  -0.231743f,   -0.23776f,  -0.246283f,  -0.224222f,  -0.201159f,   -0.20517f,  -0.263831f,  -0.153528f,  -0.150519f,  -0.131968f,  -0.191633f,  -0.214195f},
		{  0.317553f,   0.311482f,   0.312156f,   0.320251f,   0.320926f,   0.319577f,   0.326323f,   0.333743f,   0.334418f,   0.320926f,   0.300688f,   0.223785f,   0.103708f,  0.0328759f,  0.0632324f,   0.075375f,   0.107081f,   0.152278f,   0.180611f,   0.188706f,   0.212317f,   0.285173f,   0.306759f,   0.316878f,   0.328346f,   0.320251f,    0.32295f,   0.324299f,    0.32295f,   0.323624f,   0.316204f,   0.310807f,   0.306759f,   0.303386f,   0.301363f,   0.299339f,   0.302712f,   0.299339f,   0.298664f,   0.300014f,   0.295966f,    0.29799f,   0.300688f,    0.29799f,   0.295966f,   0.297315f,   0.299339f,   0.300014f,   0.302712f,    0.30541f,   0.300688f,   0.302712f,   0.302037f,   0.303386f},
		{-0.0900991f, -0.0336799f, -0.0401278f,   -0.11804f,    -0.1261f,  -0.114279f,  -0.198102f,  -0.317388f,  -0.339418f,  -0.162101f, -0.000365719f,   0.373076f,   0.851833f,    1.11888f,     1.0544f,   0.980253f,    0.86043f,   0.754039f,   0.654097f,   0.639589f,    0.53481f,   0.169429f, 0.00984347f, -0.0874125f,  -0.219057f,  -0.122339f,   -0.14813f,  -0.166399f,  -0.152966f,  -0.157802f, -0.0788152f, -0.0234707f, 0.00930614f,  0.0372471f,  0.0533668f,  0.0716359f,   0.043695f,  0.0694866f,  0.0775465f,  0.0646507f,  0.0942036f,  0.0802331f,  0.0587401f,  0.0802331f,  0.0968902f,   0.086681f,  0.0684119f,  0.0684119f,  0.0463816f,  0.0227393f,  0.0625014f,   0.043695f,  0.0512175f,  0.0415457f},
		{-0.0764866f,  -0.143781f,  -0.159504f,  -0.105102f,   -0.13875f,  -0.125228f, -0.0711408f,  -0.084977f,   0.383568f,   0.584507f,   0.641424f,   0.719725f,   0.541426f,   0.241432f, -0.0937819f,  -0.548805f,  -0.762323f,  -0.649746f,  -0.643457f, -0.0623359f,  0.0901769f, -0.00416095f,  -0.177114f,  -0.322708f,  -0.364846f,   -0.31579f,  -0.257301f,  -0.262018f,  -0.209189f,  -0.204472f,  -0.170196f,  -0.124599f, -0.0632793f,  -0.103216f,  -0.103845f,  -0.163592f,   -0.17334f,  -0.213905f,  -0.173655f,   -0.12963f,  -0.159818f,  -0.145668f,  -0.157303f,  -0.156359f,   -0.18529f,  -0.175541f,  -0.151328f,   -0.17617f,  -0.168938f,  -0.129001f,  -0.131517f,  -0.119568f,  -0.148498f,  -0.146297f},
		{ 0.0618895f,  0.0580841f,  0.0562765f,  0.0568473f,  0.0524712f,  0.0489512f,  0.0450507f,  0.0377254f,   0.038201f,  0.0330638f,  0.0313514f,  0.0391524f,  0.0309708f, 0.00357219f, -0.0285832f, -0.0815729f,  -0.101266f, -0.0808119f, -0.0811924f,  -0.022114f, -0.0116493f, -0.0216384f,  -0.034101f, -0.0438046f, -0.0530327f, -0.0615947f, -0.0692055f, -0.0786238f, -0.0861394f, -0.0945112f,  -0.101171f,  -0.106213f,   -0.10802f,  -0.112872f,  -0.114204f,  -0.118866f,  -0.119151f,  -0.121339f,  -0.119246f,  -0.117153f,  -0.119531f,  -0.119151f,  -0.120007f,  -0.119912f,   -0.12191f,  -0.122005f,  -0.121339f,  -0.123622f,  -0.123813f,   -0.12229f,  -0.123051f,  -0.122671f,  -0.124478f,  -0.124288f},
		{-0.0545484f, -0.0334675f, -0.0399539f,  -0.104278f,  -0.105899f,  -0.122115f,  -0.190223f,  -0.285357f,  -0.365357f,  -0.240493f, -0.0950886f,   0.222206f,   0.717337f,    1.06004f,    1.08598f,    1.02166f,   0.919498f,   0.823823f,    0.65896f,   0.667068f,    0.51842f,   0.238422f,  0.0649101f, -0.0513052f,  -0.170223f,  -0.124818f,  -0.139413f,  -0.172926f,  -0.171845f,  -0.152385f,  -0.112926f, -0.0437377f, -0.00103531f,  0.0319374f,   0.040586f,  0.0719371f,  0.0605858f,  0.0697749f,  0.0838289f,  0.0784235f,  0.0946396f,  0.0713966f,  0.0681533f,  0.0730182f,  0.0978828f,  0.0913964f,  0.0746398f,  0.0816667f,  0.0378833f,  0.0249104f,  0.0616669f,  0.0524778f,  0.0605858f,  0.0568021f}
	};
    int exp_result_code = 0;
    int result = knn_classifier(circle_ccw_test_sample);

    check_exact_value(knn_classifier(circle_ccw_test_sample), exp_result_code, __FUNCTION__);

    return result;
};

int circle_ccw_test1(void) {
    const float circle_ccw_test1_sample[FEATURES][DTW_SEQUENCE_LEN] = {
		{-0.0647563f, -0.0270432f, -0.0270432f, -0.0647563f, -0.0270432f, -0.0270432f, -0.0270432f,  0.0106699f, -0.0270432f,  0.0106699f,  0.0106699f, -0.0270432f,  -0.102469f, -0.0270432f, -0.0270432f,  -0.102469f,  -0.215609f,  -0.215609f,  -0.140182f,  -0.328748f,  -0.441887f,  -0.328748f, -0.0270432f,   0.236948f,   0.463227f,    0.50094f,   0.689505f,   0.840357f,   0.689505f,   0.576366f,   0.651792f,   0.425514f,   0.199235f,   0.236948f,   0.123809f,  -0.140182f,  -0.328748f,  -0.328748f,  -0.441887f,  -0.253322f,  -0.140182f,  0.0106699f, -0.0270432f,  0.0106699f,   0.123809f,   0.236948f,   0.199235f,   0.199235f,   0.199235f,   0.161522f,   0.161522f,   0.086096f,   0.123809f,  0.0483829f},
		{-0.0446449f,  -0.171663f, -0.0763994f,  -0.108154f, -0.0763994f, -0.0446449f, -0.0446449f, -0.0446449f, -0.0763994f,  -0.108154f,   0.145882f,  -0.139908f,  -0.266926f,  -0.108154f,  -0.203417f,  -0.139908f,  -0.616226f,  -0.806753f,  -0.266926f,   0.939745f,   0.939745f,   0.749218f,    1.09852f,   0.939745f,   0.590445f,   0.241146f,  -0.171663f,  -0.425699f,   -0.71149f,    -1.4736f,  -0.870262f,  -0.616226f,   -1.06079f,   -1.15605f,   -1.15605f,   -0.99728f,  -0.870262f,  -0.933771f,  -0.298681f,  -0.139908f,  -0.203417f,  -0.298681f,  -0.298681f,  -0.298681f,  -0.235172f,  -0.108154f,  -0.203417f,  -0.235172f,  -0.139908f,  -0.203417f,  -0.298681f,  -0.203417f,  -0.330435f,  -0.203417f},
		{  0.441733f,   0.411289f,   0.380845f,   0.411289f,   0.350401f,   0.350401f,   0.472177f,   0.411289f,   0.380845f,   0.380845f,   0.228625f,   0.319957f,   0.350401f,   0.350401f,   0.441733f,   0.350401f,   0.350401f,   0.289513f,     1.1115f,    1.53772f,    1.08106f,   0.472177f,  0.0155173f,  -0.106259f, -0.0758147f, -0.0758147f,  -0.197591f,  -0.380255f,  -0.867358f,  -0.319367f,  0.0764053f,   0.106849f,   0.411289f,   0.685285f,   0.928837f,   0.898393f,   0.867949f,   0.807061f,   0.715729f,   0.654841f,   0.441733f,   0.289513f,   0.198181f,   0.228625f,   0.319957f,   0.289513f,   0.350401f,   0.259069f,   0.259069f,   0.380845f,   0.441733f,   0.441733f,   0.502621f,   0.380845f},
		{  0.122023f,  0.0926667f,   0.155973f,   0.135404f,  0.0135832f,   0.165759f,  0.0513276f,   0.115833f,   0.084279f,   0.162164f,  0.0183761f,   0.238851f, -0.0289542f,  0.0171779f,    0.25343f,   0.126816f,   0.182934f,   0.114634f,   0.729529f,   0.605312f,   -0.09965f,   -0.09965f,  -0.271397f,  -0.187321f,  -0.119221f,   0.125618f, -0.0643021f,   0.072097f, -0.0776824f,    0.34849f,   0.355679f,   0.130411f,  -0.346886f,  -0.005988f, -0.0547162f, -0.0814768f,   0.262017f, -0.0898644f,     0.1378f,    0.46412f,   0.333911f,   0.343497f,   0.316737f,   0.391027f,   0.313142f,    0.23286f,   0.203503f,   0.220678f,   0.186528f,   0.231462f,     0.1378f,  0.0623114f,   0.100056f,  0.0950631f},
		{-0.0462421f, -0.0573789f, -0.0667027f, -0.0130909f,  -0.052458f, -0.000400253f, -0.0415803f, -0.0304435f, -0.0366594f, -0.0713645f, -0.0351054f,  0.0959454f,  0.0581323f,   0.116406f,   0.130651f,   0.125989f,   0.171831f,   0.290191f, -0.0889761f,   -0.72377f,   -1.13117f,   -1.29692f,   -1.33655f,   -1.21016f,   -1.02524f,  -0.758475f,  -0.512172f,   -0.15994f,   0.336033f,   0.907632f,   0.906078f,    1.07494f,    1.12726f,    1.01822f,   0.776581f,   0.449731f,   0.294853f, -0.0351054f,  -0.153725f,  -0.379308f,   -0.42515f,  -0.332171f,  -0.245149f,  -0.150358f,  -0.148804f,  -0.114099f,  -0.114099f, -0.0462421f,  0.0374128f,   0.129097f,   0.114852f,  0.0596863f,  0.0153984f,  0.0200603f},
		{  0.148212f,   0.125817f,  0.0517089f,   0.054152f,  0.0712539f,  0.0517089f,   0.116045f,   0.160835f,   0.111158f,  0.0317567f,  0.0789905f,   0.158392f,   0.036643f,  -0.239023f,  -0.273634f,  -0.365658f,  -0.628701f,   -1.25211f,   -2.20533f,   -2.02902f,   -1.18003f,  -0.616486f,   0.237794f,    1.18125f,    1.83926f,    2.41014f,    2.79737f,    2.83239f,    3.01848f,    1.84415f,   0.937749f,   0.880743f,   0.582681f, -0.0476448f,  -0.388054f,   -0.90681f,   -1.32662f,   -1.54243f,   -1.64667f,   -1.46059f,   -1.00616f,  -0.680821f,  -0.506952f,  -0.435287f,  -0.291143f,  -0.162065f,  -0.030543f,   0.036643f,   0.180787f,   0.250009f,   0.269961f,   0.185674f,   0.103829f, -0.0105908f},
		{-0.0622761f,  -0.179599f, -0.0973728f,  -0.121439f, -0.0788217f, -0.0617748f, -0.0768162f, -0.0597692f, -0.0768162f,  -0.107902f,   0.118723f,  -0.156034f,  -0.265837f,  -0.116927f,  -0.221715f,  -0.137483f,  -0.586721f,  -0.776744f,  -0.264333f,   0.524841f,   0.628126f,   0.617096f,    1.08338f,   0.958536f,   0.616093f,   0.278162f,  -0.167566f,  -0.450847f,   -0.88404f,   -1.54987f,  -0.860475f,  -0.613796f,  -0.975291f,  -0.988327f,  -0.940696f,  -0.846436f,  -0.757191f,  -0.816855f,  -0.289402f,  -0.160547f,  -0.215699f,  -0.298928f,  -0.287396f,  -0.304443f,  -0.231242f,  -0.128458f,  -0.215197f,  -0.241771f,  -0.160045f,   -0.19815f,  -0.287396f,  -0.215699f,  -0.322493f,  -0.213192f},
		{  0.320251f,   0.317553f,   0.316878f,   0.318902f,   0.316878f,   0.315529f,   0.318227f,   0.314855f,   0.317553f,   0.312156f,   0.312156f,   0.316204f,     0.3216f,   0.316878f,   0.317553f,   0.322275f,   0.329021f,   0.329021f,   0.331045f,   0.337116f,   0.338465f,   0.335092f,   0.308783f,   0.266959f,   0.231205f,   0.221761f,   0.172516f,  0.0922398f,    0.02613f,   0.179937f,   0.203547f,   0.250768f,   0.297315f,   0.299339f,   0.312831f,   0.329696f,   0.335767f,   0.336441f,   0.338465f,   0.332394f,   0.325648f,   0.310807f,   0.313505f,   0.309458f,   0.300014f,   0.289895f,   0.291918f,   0.288546f,   0.293268f,   0.297315f,   0.302712f,   0.306759f,   0.306085f,   0.309458f},
		{ -0.121264f,  -0.093323f, -0.0847258f,  -0.107293f, -0.0825765f, -0.0723673f, -0.0954723f, -0.0616208f, -0.0900991f, -0.0352919f, -0.0401278f, -0.0782779f,  -0.136846f, -0.0820392f, -0.0879498f,   -0.14007f,  -0.237864f,  -0.237864f,  -0.259357f,  -0.400673f,  -0.456555f,  -0.355538f, -0.0669941f,   0.207042f,   0.417674f,   0.488601f,   0.706755f,   0.968432f,    1.11082f,   0.552542f,   0.560065f,   0.362866f,  0.0673373f,  0.0533668f, -0.0562476f,  -0.243774f,  -0.356075f,   -0.37327f,  -0.435599f,  -0.291059f,   -0.18628f,  -0.027232f,  -0.050337f, -0.0148735f,  0.0646507f,   0.139339f,   0.125368f,   0.147399f,   0.117309f,   0.086681f,  0.0463816f,  0.0109181f,  0.0162914f, -0.0105749f},
		{-0.0673673f,  -0.117681f, -0.0925241f, -0.0934674f, -0.0689396f, -0.0777445f, -0.0560468f, -0.0632793f, -0.0739709f, -0.0991277f, 0.00621621f,  -0.124913f,  -0.168309f,  -0.144724f,    -0.1529f,   -0.11831f,  -0.345664f,  -0.525535f,  -0.305099f,   0.156842f,   0.359354f,   0.386398f,   0.605576f,   0.572558f,   0.443944f,   0.228539f, -0.0139092f,  -0.232459f,  -0.492831f,   -1.04754f,  -0.677104f,  -0.463272f,  -0.556981f,  -0.646602f,  -0.606036f,  -0.588741f,  -0.576163f,  -0.519874f,   -0.29252f,  -0.233402f,  -0.160762f,  -0.209503f,  -0.215478f,  -0.236546f,   -0.17334f,  -0.120196f,  -0.150385f,  -0.174284f,  -0.141894f,  -0.164535f,  -0.187491f,  -0.173969f,  -0.220509f,  -0.170824f},
		{  0.135428f,    0.13324f,   0.134001f,    0.13343f,   0.133716f,   0.132669f,    0.13324f,   0.132955f,   0.132574f,   0.130957f,   0.135333f,   0.129054f,   0.126866f,   0.124773f,   0.121063f,   0.118589f,   0.103939f,   0.088527f,  0.0758742f,   0.065124f,   0.049522f,  0.0354421f,  0.0312562f,  0.0274509f,  0.0217428f,  0.0161299f, 0.00985105f, 0.00385759f, -0.0105077f, -0.0749135f, -0.0253486f, 0.00271598f, -0.00479962f, -0.0151693f, -0.0197357f, -0.0296296f, -0.0425679f, -0.0557916f, -0.0680639f, -0.0819535f, -0.0927036f,  -0.102883f,  -0.108781f,  -0.114775f,   -0.11468f,  -0.113823f,  -0.117153f,  -0.119246f,  -0.116487f,  -0.116773f,  -0.116392f,  -0.114775f,  -0.117534f,  -0.115726f},
		{ -0.110224f, -0.0950886f, -0.0848184f, -0.0999535f, -0.0886022f, -0.0707645f,  -0.093467f, -0.0723861f, -0.0875211f, -0.0572511f, -0.0264405f, -0.0486025f,  -0.122115f, -0.0902238f, -0.0810347f,  -0.127521f,  -0.194007f,  -0.264277f,  -0.278871f,  -0.389681f,  -0.468599f,  -0.409681f,  -0.204818f,  0.0703155f,   0.324368f,   0.434096f,    0.64923f,   0.927606f,    1.08328f,   0.635176f,   0.642203f,   0.507609f,    0.26653f,   0.121666f, -0.00103531f,  -0.189142f,  -0.316709f,  -0.385357f,  -0.414005f,  -0.336168f,  -0.208601f, -0.0718456f, -0.0653591f, -0.0221162f,  0.0616669f,   0.106531f,   0.116802f,   0.138964f,   0.124369f,   0.113558f,  0.0643696f,  0.0281536f,  0.0238294f, -0.00427853f}
	};
    int exp_result_code = 1;
    int result = knn_classifier(circle_ccw_test1_sample);

    check_exact_value(knn_classifier(circle_ccw_test1_sample), exp_result_code, __FUNCTION__);

    return result;
};

void _run_knn_tests(void) {
	circle_ccw_test();
	circle_ccw_test1();
};
