#include "oqxt-falcon_setup.h"

#define TEMPALLOC
#define CRYPTO_SECRETKEYBYTES   1281
#define CRYPTO_PUBLICKEYBYTES   897
#define CRYPTO_BYTES            690
#define FALCON_KEYGEN_TEMP_9    14336

#define Q0I   12287
#define R1      4091
#define R2    10952
#define Q0I_large 18431


/*
 * Table for NTT, binary case:
 *   GMb[x] = R1*(g^rev(x)) mod q
 * where g = 7 (it is a 2048-th primitive root of 1 modulo q)
 * and rev() is the bit-reversal function over 10 bits.
 */
static const uint16_t GMb[] = {
	 4091,  7888, 11060, 11208,  6960,  4342,  6275,  9759,
	 1591,  6399,  9477,  5266,   586,  5825,  7538,  9710,
	 1134,  6407,  1711,   965,  7099,  7674,  3743,  6442,
	10414,  8100,  1885,  1688,  1364, 10329, 10164,  9180,
	12210,  6240,   997,   117,  4783,  4407,  1549,  7072,
	 2829,  6458,  4431,  8877,  7144,  2564,  5664,  4042,
	12189,   432, 10751,  1237,  7610,  1534,  3983,  7863,
	 2181,  6308,  8720,  6570,  4843,  1690,    14,  3872,
	 5569,  9368, 12163,  2019,  7543,  2315,  4673,  7340,
	 1553,  1156,  8401, 11389,  1020,  2967, 10772,  7045,
	 3316, 11236,  5285, 11578, 10637, 10086,  9493,  6180,
	 9277,  6130,  3323,   883, 10469,   489,  1502,  2851,
	11061,  9729,  2742, 12241,  4970, 10481, 10078,  1195,
	  730,  1762,  3854,  2030,  5892, 10922,  9020,  5274,
	 9179,  3604,  3782, 10206,  3180,  3467,  4668,  2446,
	 7613,  9386,   834,  7703,  6836,  3403,  5351, 12276,
	 3580,  1739, 10820,  9787, 10209,  4070, 12250,  8525,
	10401,  2749,  7338, 10574,  6040,   943,  9330,  1477,
	 6865,  9668,  3585,  6633, 12145,  4063,  3684,  7680,
	 8188,  6902,  3533,  9807,  6090,   727, 10099,  7003,
	 6945,  1949,  9731, 10559,  6057,   378,  7871,  8763,
	 8901,  9229,  8846,  4551,  9589, 11664,  7630,  8821,
	 5680,  4956,  6251,  8388, 10156,  8723,  2341,  3159,
	 1467,  5460,  8553,  7783,  2649,  2320,  9036,  6188,
	  737,  3698,  4699,  5753,  9046,  3687,    16,   914,
	 5186, 10531,  4552,  1964,  3509,  8436,  7516,  5381,
	10733,  3281,  7037,  1060,  2895,  7156,  8887,  5357,
	 6409,  8197,  2962,  6375,  5064,  6634,  5625,   278,
	  932, 10229,  8927,  7642,   351,  9298,   237,  5858,
	 7692,  3146, 12126,  7586,  2053, 11285,  3802,  5204,
	 4602,  1748, 11300,   340,  3711,  4614,   300, 10993,
	 5070, 10049, 11616, 12247,  7421, 10707,  5746,  5654,
	 3835,  5553,  1224,  8476,  9237,  3845,   250, 11209,
	 4225,  6326,  9680, 12254,  4136,  2778,   692,  8808,
	 6410,  6718, 10105, 10418,  3759,  7356, 11361,  8433,
	 6437,  3652,  6342,  8978,  5391,  2272,  6476,  7416,
	 8418, 10824, 11986,  5733,   876,  7030,  2167,  2436,
	 3442,  9217,  8206,  4858,  5964,  2746,  7178,  1434,
	 7389,  8879, 10661, 11457,  4220,  1432, 10832,  4328,
	 8557,  1867,  9454,  2416,  3816,  9076,   686,  5393,
	 2523,  4339,  6115,   619,   937,  2834,  7775,  3279,
	 2363,  7488,  6112,  5056,   824, 10204, 11690,  1113,
	 2727,  9848,   896,  2028,  5075,  2654, 10464,  7884,
	12169,  5434,  3070,  6400,  9132, 11672, 12153,  4520,
	 1273,  9739, 11468,  9937, 10039,  9720,  2262,  9399,
	11192,   315,  4511,  1158,  6061,  6751, 11865,   357,
	 7367,  4550,   983,  8534,  8352, 10126,  7530,  9253,
	 4367,  5221,  3999,  8777,  3161,  6990,  4130, 11652,
	 3374, 11477,  1753,   292,  8681,  2806, 10378, 12188,
	 5800, 11811,  3181,  1988,  1024,  9340,  2477, 10928,
	 4582,  6750,  3619,  5503,  5233,  2463,  8470,  7650,
	 7964,  6395,  1071,  1272,  3474, 11045,  3291, 11344,
	 8502,  9478,  9837,  1253,  1857,  6233,  4720, 11561,
	 6034,  9817,  3339,  1797,  2879,  6242,  5200,  2114,
	 7962,  9353, 11363,  5475,  6084,  9601,  4108,  7323,
	10438,  9471,  1271,   408,  6911,  3079,   360,  8276,
	11535,  9156,  9049, 11539,   850,  8617,   784,  7919,
	 8334, 12170,  1846, 10213, 12184,  7827, 11903,  5600,
	 9779,  1012,   721,  2784,  6676,  6552,  5348,  4424,
	 6816,  8405,  9959,  5150,  2356,  5552,  5267,  1333,
	 8801,  9661,  7308,  5788,  4910,   909, 11613,  4395,
	 8238,  6686,  4302,  3044,  2285, 12249,  1963,  9216,
	 4296, 11918,   695,  4371,  9793,  4884,  2411, 10230,
	 2650,   841,  3890, 10231,  7248,  8505, 11196,  6688,
	 4059,  6060,  3686,  4722, 11853,  5816,  7058,  6868,
	11137,  7926,  4894, 12284,  4102,  3908,  3610,  6525,
	 7938,  7982, 11977,  6755,   537,  4562,  1623,  8227,
	11453,  7544,   906, 11816,  9548, 10858,  9703,  2815,
	11736,  6813,  6979,   819,  8903,  6271, 10843,   348,
	 7514,  8339,  6439,   694,   852,  5659,  2781,  3716,
	11589,  3024,  1523,  8659,  4114, 10738,  3303,  5885,
	 2978,  7289, 11884,  9123,  9323, 11830,    98,  2526,
	 2116,  4131, 11407,  1844,  3645,  3916,  8133,  2224,
	10871,  8092,  9651,  5989,  7140,  8480,  1670,   159,
	10923,  4918,   128,  7312,   725,  9157,  5006,  6393,
	 3494,  6043, 10972,  6181, 11838,  3423, 10514,  7668,
	 3693,  6658,  6905, 11953, 10212, 11922,  9101,  8365,
	 5110,    45,  2400,  1921,  4377,  2720,  1695,    51,
	 2808,   650,  1896,  9997,  9971, 11980,  8098,  4833,
	 4135,  4257,  5838,  4765, 10985, 11532,   590, 12198,
	  482, 12173,  2006,  7064, 10018,  3912, 12016, 10519,
	11362,  6954,  2210,   284,  5413,  6601,  3865, 10339,
	11188,  6231,   517,  9564, 11281,  3863,  1210,  4604,
	 8160, 11447,   153,  7204,  5763,  5089,  9248, 12154,
	11748,  1354,  6672,   179,  5532,  2646,  5941, 12185,
	  862,  3158,   477,  7279,  5678,  7914,  4254,   302,
	 2893, 10114,  6890,  9560,  9647, 11905,  4098,  9824,
	10269,  1353, 10715,  5325,  6254,  3951,  1807,  6449,
	 5159,  1308,  8315,  3404,  1877,  1231,   112,  6398,
	11724, 12272,  7286,  1459, 12274,  9896,  3456,   800,
	 1397, 10678,   103,  7420,  7976,   936,   764,   632,
	 7996,  8223,  8445,  7758, 10870,  9571,  2508,  1946,
	 6524, 10158,  1044,  4338,  2457,  3641,  1659,  4139,
	 4688,  9733, 11148,  3946,  2082,  5261,  2036, 11850,
	 7636, 12236,  5366,  2380,  1399,  7720,  2100,  3217,
	10912,  8898,  7578, 11995,  2791,  1215,  3355,  2711,
	 2267,  2004,  8568, 10176,  3214,  2337,  1750,  4729,
	 4997,  7415,  6315, 12044,  4374,  7157,  4844,   211,
	 8003, 10159,  9290, 11481,  1735,  2336,  5793,  9875,
	 8192,   986,  7527,  1401,   870,  3615,  8465,  2756,
	 9770,  2034, 10168,  3264,  6132,    54,  2880,  4763,
	11805,  3074,  8286,  9428,  4881,  6933,  1090, 10038,
	 2567,   708,   893,  6465,  4962, 10024,  2090,  5718,
	10743,   780,  4733,  4623,  2134,  2087,  4802,   884,
	 5372,  5795,  5938,  4333,  6559,  7549,  5269, 10664,
	 4252,  3260,  5917, 10814,  5768,  9983,  8096,  7791,
	 6800,  7491,  6272,  1907, 10947,  6289, 11803,  6032,
	11449,  1171,  9201,  7933,  2479,  7970, 11337,  7062,
	 8911,  6728,  6542,  8114,  8828,  6595,  3545,  4348,
	 4610,  2205,  6999,  8106,  5560, 10390,  9321,  2499,
	 2413,  7272,  6881, 10582,  9308,  9437,  3554,  3326,
	 5991, 11969,  3415, 12283,  9838, 12063,  4332,  7830,
	11329,  6605, 12271,  2044, 11611,  7353, 11201, 11582,
	 3733,  8943,  9978,  1627,  7168,  3935,  5050,  2762,
	 7496, 10383,   755,  1654, 12053,  4952, 10134,  4394,
	 6592,  7898,  7497,  8904, 12029,  3581, 10748,  5674,
	10358,  4901,  7414,  8771,   710,  6764,  8462,  7193,
	 5371,  7274, 11084,   290,  7864,  6827, 11822,  2509,
	 6578,  4026,  5807,  1458,  5721,  5762,  4178,  2105,
	11621,  4852,  8897,  2856, 11510,  9264,  2520,  8776,
	 7011,  2647,  1898,  7039,  5950, 11163,  5488,  6277,
	 9182, 11456,   633, 10046, 11554,  5633,  9587,  2333,
	 7008,  7084,  5047,  7199,  9865,  8997,   569,  6390,
	10845,  9679,  8268, 11472,  4203,  1997,     2,  9331,
	  162,  6182,  2000,  3649,  9792,  6363,  7557,  6187,
	 8510,  9935,  5536,  9019,  3706, 12009,  1452,  3067,
	 5494,  9692,  4865,  6019,  7106,  9610,  4588, 10165,
	 6261,  5887,  2652, 10172,  1580, 10379,  4638,  9949
};

/*
 * Table for inverse NTT, binary case:
 *   iGMb[x] = R1*((1/g)^rev(x)) mod q
 * Since g = 7, 1/g = 8778 mod 12289.
 */
static const uint16_t iGMb[] = {
	 4091,  4401,  1081,  1229,  2530,  6014,  7947,  5329,
	 2579,  4751,  6464, 11703,  7023,  2812,  5890, 10698,
	 3109,  2125,  1960, 10925, 10601, 10404,  4189,  1875,
	 5847,  8546,  4615,  5190, 11324, 10578,  5882, 11155,
	 8417, 12275, 10599,  7446,  5719,  3569,  5981, 10108,
	 4426,  8306, 10755,  4679, 11052,  1538, 11857,   100,
	 8247,  6625,  9725,  5145,  3412,  7858,  5831,  9460,
	 5217, 10740,  7882,  7506, 12172, 11292,  6049,    79,
	   13,  6938,  8886,  5453,  4586, 11455,  2903,  4676,
	 9843,  7621,  8822,  9109,  2083,  8507,  8685,  3110,
	 7015,  3269,  1367,  6397, 10259,  8435, 10527, 11559,
	11094,  2211,  1808,  7319,    48,  9547,  2560,  1228,
	 9438, 10787, 11800,  1820, 11406,  8966,  6159,  3012,
	 6109,  2796,  2203,  1652,   711,  7004,  1053,  8973,
	 5244,  1517,  9322, 11269,   900,  3888, 11133, 10736,
	 4949,  7616,  9974,  4746, 10270,   126,  2921,  6720,
	 6635,  6543,  1582,  4868,    42,   673,  2240,  7219,
	 1296, 11989,  7675,  8578, 11949,   989, 10541,  7687,
	 7085,  8487,  1004, 10236,  4703,   163,  9143,  4597,
	 6431, 12052,  2991, 11938,  4647,  3362,  2060, 11357,
	12011,  6664,  5655,  7225,  5914,  9327,  4092,  5880,
	 6932,  3402,  5133,  9394, 11229,  5252,  9008,  1556,
	 6908,  4773,  3853,  8780, 10325,  7737,  1758,  7103,
	11375, 12273,  8602,  3243,  6536,  7590,  8591, 11552,
	 6101,  3253,  9969,  9640,  4506,  3736,  6829, 10822,
	 9130,  9948,  3566,  2133,  3901,  6038,  7333,  6609,
	 3468,  4659,   625,  2700,  7738,  3443,  3060,  3388,
	 3526,  4418, 11911,  6232,  1730,  2558, 10340,  5344,
	 5286,  2190, 11562,  6199,  2482,  8756,  5387,  4101,
	 4609,  8605,  8226,   144,  5656,  8704,  2621,  5424,
	10812,  2959, 11346,  6249,  1715,  4951,  9540,  1888,
	 3764,    39,  8219,  2080,  2502,  1469, 10550,  8709,
	 5601,  1093,  3784,  5041,  2058,  8399, 11448,  9639,
	 2059,  9878,  7405,  2496,  7918, 11594,   371,  7993,
	 3073, 10326,    40, 10004,  9245,  7987,  5603,  4051,
	 7894,   676, 11380,  7379,  6501,  4981,  2628,  3488,
	10956,  7022,  6737,  9933,  7139,  2330,  3884,  5473,
	 7865,  6941,  5737,  5613,  9505, 11568, 11277,  2510,
	 6689,   386,  4462,   105,  2076, 10443,   119,  3955,
	 4370, 11505,  3672, 11439,   750,  3240,  3133,   754,
	 4013, 11929,  9210,  5378, 11881, 11018,  2818,  1851,
	 4966,  8181,  2688,  6205,  6814,   926,  2936,  4327,
	10175,  7089,  6047,  9410, 10492,  8950,  2472,  6255,
	  728,  7569,  6056, 10432, 11036,  2452,  2811,  3787,
	  945,  8998,  1244,  8815, 11017, 11218,  5894,  4325,
	 4639,  3819,  9826,  7056,  6786,  8670,  5539,  7707,
	 1361,  9812,  2949, 11265, 10301,  9108,   478,  6489,
	  101,  1911,  9483,  3608, 11997, 10536,   812,  8915,
	  637,  8159,  5299,  9128,  3512,  8290,  7068,  7922,
	 3036,  4759,  2163,  3937,  3755, 11306,  7739,  4922,
	11932,   424,  5538,  6228, 11131,  7778, 11974,  1097,
	 2890, 10027,  2569,  2250,  2352,   821,  2550, 11016,
	 7769,   136,   617,  3157,  5889,  9219,  6855,   120,
	 4405,  1825,  9635,  7214, 10261, 11393,  2441,  9562,
	11176,   599,  2085, 11465,  7233,  6177,  4801,  9926,
	 9010,  4514,  9455, 11352, 11670,  6174,  7950,  9766,
	 6896, 11603,  3213,  8473,  9873,  2835, 10422,  3732,
	 7961,  1457, 10857,  8069,   832,  1628,  3410,  4900,
	10855,  5111,  9543,  6325,  7431,  4083,  3072,  8847,
	 9853, 10122,  5259, 11413,  6556,   303,  1465,  3871,
	 4873,  5813, 10017,  6898,  3311,  5947,  8637,  5852,
	 3856,   928,  4933,  8530,  1871,  2184,  5571,  5879,
	 3481, 11597,  9511,  8153,    35,  2609,  5963,  8064,
	 1080, 12039,  8444,  3052,  3813, 11065,  6736,  8454,
	 2340,  7651,  1910, 10709,  2117,  9637,  6402,  6028,
	 2124,  7701,  2679,  5183,  6270,  7424,  2597,  6795,
	 9222, 10837,   280,  8583,  3270,  6753,  2354,  3779,
	 6102,  4732,  5926,  2497,  8640, 10289,  6107, 12127,
	 2958, 12287, 10292,  8086,   817,  4021,  2610,  1444,
	 5899, 11720,  3292,  2424,  5090,  7242,  5205,  5281,
	 9956,  2702,  6656,   735,  2243, 11656,   833,  3107,
	 6012,  6801,  1126,  6339,  5250, 10391,  9642,  5278,
	 3513,  9769,  3025,   779,  9433,  3392,  7437,   668,
	10184,  8111,  6527,  6568, 10831,  6482,  8263,  5711,
	 9780,   467,  5462,  4425, 11999,  1205,  5015,  6918,
	 5096,  3827,  5525, 11579,  3518,  4875,  7388,  1931,
	 6615,  1541,  8708,   260,  3385,  4792,  4391,  5697,
	 7895,  2155,  7337,   236, 10635, 11534,  1906,  4793,
	 9527,  7239,  8354,  5121, 10662,  2311,  3346,  8556,
	  707,  1088,  4936,   678, 10245,    18,  5684,   960,
	 4459,  7957,   226,  2451,     6,  8874,   320,  6298,
	 8963,  8735,  2852,  2981,  1707,  5408,  5017,  9876,
	 9790,  2968,  1899,  6729,  4183,  5290, 10084,  7679,
	 7941,  8744,  5694,  3461,  4175,  5747,  5561,  3378,
	 5227,   952,  4319,  9810,  4356,  3088, 11118,   840,
	 6257,   486,  6000,  1342, 10382,  6017,  4798,  5489,
	 4498,  4193,  2306,  6521,  1475,  6372,  9029,  8037,
	 1625,  7020,  4740,  5730,  7956,  6351,  6494,  6917,
	11405,  7487, 10202, 10155,  7666,  7556, 11509,  1546,
	 6571, 10199,  2265,  7327,  5824, 11396, 11581,  9722,
	 2251, 11199,  5356,  7408,  2861,  4003,  9215,   484,
	 7526,  9409, 12235,  6157,  9025,  2121, 10255,  2519,
	 9533,  3824,  8674, 11419, 10888,  4762, 11303,  4097,
	 2414,  6496,  9953, 10554,   808,  2999,  2130,  4286,
	12078,  7445,  5132,  7915,   245,  5974,  4874,  7292,
	 7560, 10539,  9952,  9075,  2113,  3721, 10285, 10022,
	 9578,  8934, 11074,  9498,   294,  4711,  3391,  1377,
	 9072, 10189,  4569, 10890,  9909,  6923,    53,  4653,
	  439, 10253,  7028, 10207,  8343,  1141,  2556,  7601,
	 8150, 10630,  8648,  9832,  7951, 11245,  2131,  5765,
	10343,  9781,  2718,  1419,  4531,  3844,  4066,  4293,
	11657, 11525, 11353,  4313,  4869, 12186,  1611, 10892,
	11489,  8833,  2393,    15, 10830,  5003,    17,   565,
	 5891, 12177, 11058, 10412,  8885,  3974, 10981,  7130,
	 5840, 10482,  8338,  6035,  6964,  1574, 10936,  2020,
	 2465,  8191,   384,  2642,  2729,  5399,  2175,  9396,
	11987,  8035,  4375,  6611,  5010, 11812,  9131, 11427,
	  104,  6348,  9643,  6757, 12110,  5617, 10935,   541,
	  135,  3041,  7200,  6526,  5085, 12136,   842,  4129,
	 7685, 11079,  8426,  1008,  2725, 11772,  6058,  1101,
	 1950,  8424,  5688,  6876, 12005, 10079,  5335,   927,
	 1770,   273,  8377,  2271,  5225, 10283,   116, 11807,
	   91, 11699,   757,  1304,  7524,  6451,  8032,  8154,
	 7456,  4191,   309,  2318,  2292, 10393, 11639,  9481,
	12238, 10594,  9569,  7912, 10368,  9889, 12244,  7179,
	 3924,  3188,   367,  2077,   336,  5384,  5631,  8596,
	 4621,  1775,  8866,   451,  6108,  1317,  6246,  8795,
	 5896,  7283,  3132, 11564,  4977, 12161,  7371,  1366,
	12130, 10619,  3809,  5149,  6300,  2638,  4197,  1418,
	10065,  4156,  8373,  8644, 10445,   882,  8158, 10173,
	 9763, 12191,   459,  2966,  3166,   405,  5000,  9311,
	 6404,  8986,  1551,  8175,  3630, 10766,  9265,   700,
	 8573,  9508,  6630, 11437, 11595,  5850,  3950,  4775,
	11941,  1446,  6018,  3386, 11470,  5310,  5476,   553,
	 9474,  2586,  1431,  2741,   473, 11383,  4745,   836,
	 4062, 10666,  7727, 11752,  5534,   312,  4307,  4351,
	 5764,  8679,  8381,  8187,     5,  7395,  4363,  1152,
	 5421,  5231,  6473,   436,  7567,  8603,  6229,  8230
};




string rawdb_file = "db6k.dat";
string eidxdb_file = "EDB.csv";
string bloomfilter_file = "bloom_filter.dat";


unsigned char **BF;

unsigned char *UIDX;


//db6k.dat
int N_keywords = 6043;
int N_max_ids = 9690;
int N_row_ids = N_max_ids;
int N_words = N_keywords;
int N_max_id_words = 1809;
int N_kw_id_max = 80901;


int N_threads = 16;

int sym_block_size = N_threads*16;
int hash_block_size = N_threads*64;
int bhash_block_size = N_threads*64;
int bhash_in_block_size = N_threads*40;




//IVs and Keys for AES-256 GCM encryption
unsigned char iv_ki[16], iv_r[16], iv_ke[16];// iv_kz[16], iv_kx[16], iv_stag[16];
unsigned char tag_r[100], tag_ec[100], tag_kz[100], tag_ks[100], tag_ki[100], tag_kx[100], tag_kt[100],tag_stag[100]; 
unsigned char aad[16]="00000002";
int ke, kw, ka, kec, kt, k_stag, k_stag_query, k_stag_TSetRetrieve;
int kw_dec;
vector<int> kid_enc_vec, kid_dec_vec, kr_enc_vec;


unsigned char iv_ks[16] = {0x56,0x37,0xca,0x94,0xd5,0xe0,0xad,0x62,0x73,0x7c,0xba,0x48,0x8d,0x2d,0x4d,0xde};
unsigned char iv_ec[16] = {0x56,0x37,0xca,0x94,0xd5,0xe0,0xad,0x62,0x73,0x7c,0xba,0x48,0x8d,0x2d,0x4d,0xde};
unsigned char iv_kt[16] = {0x56,0x37,0xca,0x94,0xd5,0xe0,0xad,0x62,0x73,0x7c,0xba,0x48,0x8d,0x2d,0x4d,0xde};
unsigned char iv_stag[16] = {0x56,0x37,0xca,0x94,0xd5,0xe0,0xad,0x62,0x73,0x7c,0xba,0x48,0x8d,0x2d,0x4d,0xde};
unsigned char iv_kx[16] = {0x56,0x37,0xca,0x94,0xd5,0xe0,0xad,0x62,0x73,0x7c,0xba,0x48,0x8d,0x2d,0x4d,0xde};
unsigned char iv_kz[16] = {0x56,0x37,0xca,0x94,0xd5,0xe0,0xad,0x62,0x73,0x7c,0xba,0x48,0x8d,0x2d,0x4d,0xde};


unsigned char KS[32] = {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4};
unsigned char KI[32] = {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4};
unsigned char KZ[32] = {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4};
unsigned char KX[32] = {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4};
unsigned char KR[32] = {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4};
unsigned char KT[32] = {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4};




const char* KS1 = reinterpret_cast<const char *> (KS);
const char* KZ1 = reinterpret_cast<const char *> (KZ);
const char* KI1 = reinterpret_cast<const char *> (KI);
const char* KX1 = reinterpret_cast<const char *> (KX);
const char* KR1 = reinterpret_cast<const char *> (KR);
const char* KT1 = reinterpret_cast<const char *> (KT);





//For Bloom Filter Implementation
unsigned char* GL_BLOOM_MSG = new unsigned char[40*N_max_id_words];
unsigned char* GL_BLOOM_DGST = new unsigned char[64*N_max_id_words];
unsigned char* GL_HASH_DGST = new unsigned char[64*N_max_id_words];
unsigned char* GL_HASH_MSG = new unsigned char[32*N_max_id_words];
unsigned char* GL_BLM_MSG = new unsigned char[40];
unsigned char* GL_BLM_DGST = new unsigned char[64];

unsigned char* GL_MGDB_RES = new unsigned char[N_max_id_words*49]; 
unsigned char* GL_MGDB_BIDX = new unsigned char[N_max_id_words*2];
unsigned char* GL_MGDB_JIDX = new unsigned char[N_max_id_words*2];
unsigned char* GL_MGDB_LBL = new unsigned char[N_max_id_words*12];

unsigned char* MGDB_RES;
unsigned char* MGDB_BIDX;
unsigned char* MGDB_JIDX;
unsigned char* MGDB_LBL;

sw::redis::ConnectionOptions connection_options;
sw::redis::ConnectionPoolOptions pool_options;

// // ---------------------------------------------------------------------------------------------------------------------------------- // //



int Sys_Init()
{
    
    connection_options.host = "127.0.0.1";  
    BloomFilter_Init(BF);

    return 0;
}

int Sys_Clear()
{
    BloomFilter_Clean(BF);

    return 0;
}


//For Falcon random plynomial generation (HashToPoint)
static void *
xmalloc(size_t len)
{
	void *buf;

	if (len == 0) {
		return NULL;
	}
	buf = malloc(len);
	if (buf == NULL) {
		fprintf(stderr, "memory allocation error\n");
		exit(EXIT_FAILURE);
	}
	return buf;
}


/*
 * Reduce a small signed integer modulo q. The source integer MUST
 * be between -q/2 and +q/2.
 */
static inline uint32_t
mq_conv_small(int x)
{
	/*
	 * If x < 0, the cast to uint32_t will set the high bit to 1.
	 */
	uint32_t y;

	y = (uint32_t)x;
	y += q_l & -(y >> 31);
	return y;
}


/*
 * Addition modulo q. Operands must be in the 0..q-1 range.
 */
static inline uint32_t
mq_add(uint32_t x, uint32_t y)
{
	/*
	 * We compute x + y - q. If the result is negative, then the
	 * high bit will be set, and 'd >> 31' will be equal to 1;
	 * thus '-(d >> 31)' will be an all-one pattern. Otherwise,
	 * it will be an all-zero pattern. In other words, this
	 * implements a conditional addition of q.
	 */
	uint32_t d;

	d = x + y - q_l;
	d += q_l & -(d >> 31);
	return d;
}


/*
 * Subtraction modulo q. Operands must be in the 0..q-1 range.
 */
static inline uint32_t
mq_sub(uint32_t x, uint32_t y)
{
	/*
	 * As in mq_add(), we use a conditional addition to ensure the
	 * result is in the 0..q-1 range.
	 */
	uint32_t d;

	d = x - y;
	d += q_l & -(d >> 31);
	return d;
}



/*
 * Division by 2 modulo q. Operand must be in the 0..q-1 range.
 */
static inline uint32_t
mq_rshift1(uint32_t x)
{
	x += q_l & -(x & 1);
	return (x >> 1);
}


/*
 * Montgomery multiplication modulo q. If we set R = 2^16 mod q, then
 * this function computes: x * y / R mod q
 * Operands must be in the 0..q-1 range.
 */
static inline uint32_t
mq_montymul(uint32_t x, uint32_t y)
{
	uint32_t z, w;

	/*
	 * We compute x*y + k*q with a value of k chosen so that the 16
	 * low bits of the result are 0. We can then shift the value.
	 * After the shift, result may still be larger than q, but it
	 * will be lower than 2*q, so a conditional subtraction works.
	 */

	z = x * y;
	w = ((z * Q0I) & 0xFFFF) * q_l;

	/*
	 * When adding z and w, the result will have its low 16 bits
	 * equal to 0. Since x, y and z are lower than q, the sum will
	 * be no more than (2^15 - 1) * q + (q - 1)^2, which will
	 * fit on 29 bits.
	 */
	z = (z + w) >> 16;

	/*
	 * After the shift, analysis shows that the value will be less
	 * than 2q. We do a subtraction then conditional subtraction to
	 * ensure the result is in the expected range.
	 */
	z -= q_l;
	z += q_l & -(z >> 31);
	return z;
}



/*
 * Montgomery squaring (computes (x^2)/R).
 */
static inline uint32_t
mq_montysqr(uint32_t x)
{
	return mq_montymul(x, x);
}


/*
 * Divide x by y modulo q = 12289.
 */
static inline uint32_t
mq_div_12289(uint32_t x, uint32_t y)
{
	/*
	 * We invert y by computing y^(q-2) mod q.
	 *
	 * We use the following addition chain for exponent e = 12287:
	 *
	 *   e0 = 1
	 *   e1 = 2 * e0 = 2
	 *   e2 = e1 + e0 = 3
	 *   e3 = e2 + e1 = 5
	 *   e4 = 2 * e3 = 10
	 *   e5 = 2 * e4 = 20
	 *   e6 = 2 * e5 = 40
	 *   e7 = 2 * e6 = 80
	 *   e8 = 2 * e7 = 160
	 *   e9 = e8 + e2 = 163
	 *   e10 = e9 + e8 = 323
	 *   e11 = 2 * e10 = 646
	 *   e12 = 2 * e11 = 1292
	 *   e13 = e12 + e9 = 1455
	 *   e14 = 2 * e13 = 2910
	 *   e15 = 2 * e14 = 5820
	 *   e16 = e15 + e10 = 6143
	 *   e17 = 2 * e16 = 12286
	 *   e18 = e17 + e0 = 12287
	 *
	 * Additions on exponents are converted to Montgomery
	 * multiplications. We define all intermediate results as so
	 * many local variables, and let the C compiler work out which
	 * must be kept around.
	 */
	uint32_t y0, y1, y2, y3, y4, y5, y6, y7, y8, y9;
	uint32_t y10, y11, y12, y13, y14, y15, y16, y17, y18;

	y0 = mq_montymul(y, R2);
	y1 = mq_montysqr(y0);
	y2 = mq_montymul(y1, y0);
	y3 = mq_montymul(y2, y1);
	y4 = mq_montysqr(y3);
	y5 = mq_montysqr(y4);
	y6 = mq_montysqr(y5);
	y7 = mq_montysqr(y6);
	y8 = mq_montysqr(y7);
	y9 = mq_montymul(y8, y2);
	y10 = mq_montymul(y9, y8);
	y11 = mq_montysqr(y10);
	y12 = mq_montysqr(y11);
	y13 = mq_montymul(y12, y9);
	y14 = mq_montysqr(y13);
	y15 = mq_montysqr(y14);
	y16 = mq_montymul(y15, y10);
	y17 = mq_montysqr(y16);
	y18 = mq_montymul(y17, y0);

	/*
	 * Final multiplication with x, which is not in Montgomery
	 * representation, computes the correct division result.
	 */
	return mq_montymul(y18, x);
}



/*
 * Compute NTT on a ring element.
 */
static void
mq_NTT(uint16_t *a, unsigned logn)
{
	size_t n, t, m;

	n = (size_t)1 << logn;
	t = n;
	for (m = 1; m < n; m <<= 1) {
		size_t ht, i, j1;

		ht = t >> 1;
		for (i = 0, j1 = 0; i < m; i ++, j1 += t) {
			size_t j, j2;
			uint32_t s;

			s = GMb[m + i];
			j2 = j1 + ht;
			for (j = j1; j < j2; j ++) {
				uint32_t u, v;

				u = a[j];
				v = mq_montymul(a[j + ht], s);
				a[j] = (uint16_t)mq_add(u, v);
				a[j + ht] = (uint16_t)mq_sub(u, v);
			}
		}
		t = ht;
	}
}


/*
 * Compute the inverse NTT on a ring element, binary case.
 */
static void
mq_iNTT(uint16_t *a, unsigned logn)
{
	size_t n, t, m;
	uint32_t ni;

	n = (size_t)1 << logn;
	t = 1;
	m = n;
	while (m > 1) {
		size_t hm, dt, i, j1;

		hm = m >> 1;
		dt = t << 1;
		for (i = 0, j1 = 0; i < hm; i ++, j1 += dt) {
			size_t j, j2;
			uint32_t s;

			j2 = j1 + t;
			s = iGMb[hm + i];
			for (j = j1; j < j2; j ++) {
				uint32_t u, v, w;

				u = a[j];
				v = a[j + t];
				a[j] = (uint16_t)mq_add(u, v);
				w = mq_sub(u, v);
				a[j + t] = (uint16_t)
					mq_montymul(w, s);
			}
		}
		t = dt;
		m = hm;
	}

	/*
	 * To complete the inverse NTT, we must now divide all values by
	 * n (the vector size). We thus need the inverse of n, i.e. we
	 * need to divide 1 by 2 logn times. But we also want it in
	 * Montgomery representation, i.e. we also want to multiply it
	 * by R = 2^16. In the common case, this should be a simple right
	 * shift. The loop below is generic and works also in corner cases;
	 * its computation time is negligible.
	 */
	ni = R1;
	for (m = n; m > 1; m >>= 1) {
		ni = mq_rshift1(ni);
	}
	for (m = 0; m < n; m ++) {
		a[m] = (uint16_t)mq_montymul(a[m], ni);
	}
}


/*
 * Convert a polynomial (mod q) to Montgomery representation.
 */
static void
mq_poly_tomonty(uint16_t *f, unsigned logn)
{
	size_t u, n;

	n = (size_t)1 << logn;
	for (u = 0; u < n; u ++) {
		f[u] = (uint16_t)mq_montymul(f[u], R2);
	}
}


/*
 * Multiply two polynomials together (NTT representation, and using
 * a Montgomery multiplication). Result f*g is written over f.
 */
static void
mq_poly_montymul_ntt(uint16_t *f, const uint16_t *g, unsigned logn)
{
	size_t u, n;

	n = (size_t)1 << logn;
	for (u = 0; u < n; u ++) {
		f[u] = (uint16_t)mq_montymul(f[u], g[u]);
	}
}

/*
 * Subtract polynomial g from polynomial f.
 */
static void
mq_poly_sub(uint16_t *f, const uint16_t *g, unsigned logn)
{
	size_t u, n;

	n = (size_t)1 << logn;
	for (u = 0; u < n; u ++) {
		f[u] = (uint16_t)mq_sub(f[u], g[u]);
	}
}


/* ===================================================================== */


int SHA3_HASH(blake3_hasher *hasher,unsigned char *msg, unsigned char *digest)
{
    Blake3(hasher,digest,msg);
    return 0;
}

int SHA3_HASH_K(blake3_hasher *hasher,unsigned char *msg, unsigned char *digest)
{
    Blake3_K(hasher,digest,msg);
    return 0;
}


int BLOOM_HASH(unsigned char *msg, unsigned char *digest)
{
        blake3_hasher hasher;
        blake3_hasher_init(&hasher);


        ::memset(GL_BLOOM_MSG,0x00,bhash_in_block_size);
        ::memset(GL_BLOOM_DGST,0x00,hash_block_size);
        for(int i=0;i<N_HASH;i++){
            ::memcpy(GL_BLOOM_MSG+(40*i),msg,32);
            GL_BLOOM_MSG[40*i+39] = (i & 0xFF);
        }

   
        SHA3_HASH_K(&hasher,GL_BLOOM_MSG,GL_BLOOM_DGST);
        
        ::memcpy(digest,GL_BLOOM_DGST,hash_block_size);
        
    return 0;
}


int FPGA_HASH(unsigned char *msg, unsigned char *digest)
{
    blake3_hasher hasher;
    blake3_hasher_init(&hasher);
    
    ::memset(GL_HASH_DGST,0x00,hash_block_size);
    ::memcpy(GL_HASH_MSG,msg,EVP_MAX_BLOCK_LENGTH);

  
    SHA3_HASH(&hasher,GL_HASH_MSG,GL_HASH_DGST);

    ::memcpy(digest,GL_HASH_DGST,hash_block_size);

    return 0;
}

int MGDB_QUERY(unsigned char *RES, unsigned char *BIDX, unsigned char *JIDX, unsigned char *LBL)
{


    auto redis = Redis("tcp://127.0.0.1:6379");

    ::memcpy(GL_MGDB_BIDX,BIDX,(2));
    ::memcpy(GL_MGDB_JIDX,JIDX,(2));
    ::memcpy(GL_MGDB_LBL,LBL,(12));
    ::memset(GL_MGDB_RES,0x00,(49));
        
        
    string s = HexToStr(MGDB_BIDX,2) + HexToStr(MGDB_JIDX,2) + HexToStr(MGDB_LBL,12);
    auto val = redis.get(s);
    unsigned char *t_res = reinterpret_cast<unsigned char *>(val->data());
    DB_StrToHex49(MGDB_RES,t_res);

    // ::memcpy(RES,GL_MGDB_RES,(N_threads*49));
    ::memcpy(RES,GL_MGDB_RES,(49));

    return 0;
}




int TSet_SetUp()
{
    unsigned char *W;
    unsigned char *TW;
    unsigned char *stag;
    unsigned char *stagi;
    unsigned char *stago;
    unsigned char *hashin;
    unsigned char *hashout;
    
    int N_words = 0;
    unsigned int N_max_id_words = 0;
    
    N_words = (N_max_ids/N_threads) + ((N_max_ids%N_threads==0)?0:1);
    N_max_id_words = N_words * N_threads;

    auto redis = Redis("tcp://127.0.0.1:6379");
    
    int datasize = (2*N_l) + 16;

    TW = new unsigned char[datasize*N_max_id_words];
    W = new unsigned char[16*N_max_id_words];
    stag = new unsigned char[16*N_max_id_words];
    stagi = new unsigned char[16*N_max_id_words];
    stago = new unsigned char[16*N_max_id_words];
    hashin = new unsigned char[16*N_max_id_words];
    hashout = new unsigned char[64*N_max_id_words];

    //To store TSet Value -- single execution
    unsigned char TVAL[(datasize+1)];
    unsigned char TBIDX[2];
    unsigned char TJIDX[2];
    unsigned char TLBL[12];

    unsigned int *FreeB;
    int bidx=0;
    int len_freeb = 65536;
    int total_count = 0;
    int freeb_idx = 0;


    FreeB = new unsigned int[len_freeb];

    ifstream eidxdb_file_handle;
    eidxdb_file_handle.open(eidxdb_file,ios_base::in|ios_base::binary);

    stringstream ss;

    string eidxdb_row;
    vector<string> eidxdb_data;
    string eidxdb_row_current;
    string s;

    ::memset(hashin,0x00,16*N_max_id_words);
    ::memset(hashout,0x00,64*N_max_id_words);
    ::memset(TJIDX,0x00,2);

    for(int bc=0;bc<len_freeb;++bc){
        FreeB[bc] = 0;
    }

    int n_rows = 0;
    int n_row_ids = 0;

    eidxdb_row.clear();
    while(getline(eidxdb_file_handle,eidxdb_row)){
        eidxdb_data.push_back(eidxdb_row);
        eidxdb_row.clear();
        n_rows++;
    }

    eidxdb_file_handle.close();

    int current_row_len = 0;

    unsigned char *tw_local = TW;
    unsigned char *w_local = W;
    unsigned char *stag_local = stag;
    unsigned char *stagi_local = stagi;
    unsigned char *hashin_local = hashin;
    unsigned char *hashout_local = hashout;
    
    unsigned long id_count = 0;

    std::string db_in_key = "";
    std::string db_in_val = "";

    for(int n=0;n<n_rows;++n){

        ::memset(W,0x00,16*N_max_id_words);
        ::memset(TW,0x00,datasize*N_max_id_words);
        ::memset(stag,0x00,16*N_max_id_words);
        ::memset(stagi,0x00,16*N_max_id_words);
        ::memset(stago,0x00,16*N_max_id_words);
        ::memset(hashin,0x00,16*N_max_id_words);
        ::memset(hashout,0x00,64*N_max_id_words);

        eidxdb_row_current = eidxdb_data.at(n);

        ss.str(std::string());
        ss << eidxdb_row_current;

        std::getline(ss,s,',');//Get the keyword
        DB_StrToHex8(W,s.data());//Read the keyword
        

        tw_local = TW;
        n_row_ids = 0;
        while(std::getline(ss,s,',') && !ss.eof()) {
            if(!s.empty()){
                DB_StrToHexN(tw_local,s.data(),datasize);//Read the id
                tw_local += datasize;
                n_row_ids++;
            }
        }

        ss.clear();
        ss.seekg(0);

        tw_local = TW;

        N_words = (n_row_ids/N_threads) + ((n_row_ids%N_threads==0)?0:1);


        stag_local = stag;
        w_local = W;
        kt = encrypt(w_local, sizeof(w_local)/sizeof(w_local[0]), aad, sizeof(aad), KT1, iv_kt, stag_local, tag_kt);
        stag_local += EVP_MAX_BLOCK_LENGTH;
        w_local += 16;
      
        stag_local = stag;
        w_local = W;
        

        //Fill stagi array
        stagi_local = stagi;
        for(int nword = 0;nword < N_words;++nword){
            for(int nid=0;nid<N_threads;nid++){
                stagi_local[0] = ((nword*N_threads)+nid) & 0xFF; 
                stagi_local += 16;
            }
        }
        stagi_local = stagi;

     
        //PRF of stag and i
        const char* stag1 = reinterpret_cast<const char *> (stag);
        if(!PKCS5_PBKDF2_HMAC_SHA1(stag, strlen(stag),NULL,0,1000,32,stag1))
        {
            printf("Error in key generation\n");
            exit(1);
        }

        stagi_local = stagi;
        hashin_local = hashin;
        for(int nword = 0;nword < N_words;++nword){
            k_stag = encrypt(stagi_local, sizeof(stagi_local)/sizeof(stagi_local[0]), aad, sizeof(aad), stag1, iv_stag, hashin_local, tag_stag);
            stagi_local += 16;
            hashin_local += 16;
        }
        stagi_local = stagi;
        hashin_local = hashin;
        

        //Compute Hash
        hashin_local = hashin;
        hashout_local = hashout;
        for(int nword = 0;nword < N_words;++nword){
            FPGA_HASH(hashin_local,hashout_local);
            hashin_local += 16;
            hashout_local += hash_block_size;
        }
        hashin_local = hashin;
        hashout_local = hashout;



        //Should be done for each stag
        for(int bc=0;bc<len_freeb;++bc){
            FreeB[bc] = 0;
        }

        tw_local = TW;
        

        for(int i=0;i<n_row_ids;++i){
            ::memcpy(TVAL+1,tw_local,datasize);
           

            TVAL[0] = (i==(n_row_ids-1))?0x01:0x00;
            for(int j=0;j<datasize+1;++j){
                // TVAL[j] = hashout[64*i+15+j] ^ TVAL[j];
                TVAL[j] = 0 ^ TVAL[j];
            }
        
            
            ::memcpy(TBIDX,(hashout+(64*i)),2);
            ::memcpy(TLBL,(hashout+(64*i)+2),12);

            freeb_idx = (TBIDX[1] << 8) + TBIDX[0];

            bidx = (FreeB[freeb_idx]++);
            TJIDX[0] =  bidx & 0xFF;
            TJIDX[1] =  (bidx >> 8) & 0xFF;

            
            db_in_key.clear();
            db_in_val.clear();
            db_in_key = HexToStr(TBIDX,2) + HexToStr(TJIDX,2) + HexToStr(TLBL,12);
            db_in_val = HexToStr(TVAL,datasize+1);
            redis.set(db_in_key.data(), db_in_val.data());

            tw_local += datasize;
            total_count++;
        }
    
    }
 
    std::cout << "Total ID Count: " << total_count << std::endl;

    delete [] TW;
    delete [] W;
    delete [] stag;
    delete [] stagi;
    delete [] stago;
    delete [] hashin;
    delete [] hashout;

    delete [] FreeB;

    return 0;
}


int FPGA_BLOOM_HASH(unsigned char *msg, unsigned char *digest)
{

    blake3_hasher hasher;
    blake3_hasher_init(&hasher);
    
    ::memset(GL_BLM_MSG,0x00,bhash_in_block_size);
    ::memset(GL_BLM_DGST,0x00,bhash_block_size);
    for(int i=0;i<N_HASH;i++){
        ::memcpy(GL_BLM_MSG+(40*i),msg,32);
        GL_BLM_MSG[40*i+39] = (i & 0xFF);
    }

    SHA3_HASH_K(&hasher,GL_BLM_MSG,GL_BLM_DGST);

    ::memcpy(digest,GL_BLM_DGST,bhash_block_size);

    return 0;
}


unsigned int BFIdxConv(unsigned char *hex_arr,unsigned int n_bits)
{
    unsigned int idx_val = 0;
    unsigned int n_bytes = n_bits/8;
    unsigned int n_bits_rem = n_bits%8;
    unsigned char tmp_char;
    
    for(unsigned int i=0;i<n_bytes;++i){
        idx_val = (idx_val << 8) | hex_arr[i];
    }

    if(n_bits_rem != 0){
        tmp_char = hex_arr[n_bytes];
        tmp_char = tmp_char >> (8 - n_bits_rem);
        idx_val = (idx_val << n_bits_rem) | tmp_char;
    }

    return idx_val;
}


static void
mk_rand_poly_oqxt(prng *p, fpr *f, unsigned logn)
{
	size_t u, n;

	n = (size_t)1 << logn;
	for (u = 0; u < n; u ++) {
		int32_t x;
		
		x = prng_get_u8(p);
		x = (x << 8) + prng_get_u8(p);
		x &= 0x3FF;
		f[u] = fpr_of(x - 512);
	}
}


int main()   
{
    unsigned char *W;
    unsigned char *ID;
    uint16_t *XID;        

    unsigned char *KE_temp;
    unsigned char *KE;
    unsigned char *EC;
    
    unsigned char *WC;
    unsigned char *R;
    unsigned char *YID_char;

    int16_t *XTAG;    
    unsigned char *bhash;

    uint16_t *YID;


    ///////////////////////////////////////////////////////////////////
    
    W = new unsigned char[16];                                              //Holds the keyword 16B
    ID = new unsigned char[N_max_id_words*16];                              //Maximum number of IDs in a row 16B
    KE = new unsigned char[EVP_MAX_BLOCK_LENGTH];                           //ID encryption key 32B
    KE_temp = new unsigned char[16];                                        //ID encryption key 16B
    WC = new unsigned char[N_max_id_words*16];                              //IDs with counter value (for computing randomness R) 
    R = new unsigned char[N_max_id_words*16];                               //Random value for generating trapdoor matrix - used in generating A
    EC = new unsigned char[N_max_id_words*16];                              //Encrypted IDs
    bhash = new unsigned char[bhash_block_size];
    
    XTAG = new int16_t[N_max_id_words*N_l];
    XID = new uint16_t[N_max_id_words*N_l];
    YID_char = new unsigned char[N_max_id_words*N_l*2];                        
    YID = new uint16_t [N_max_id_words*N_l];                                 //YID = n x 1 vector (short vector) for every (W-ID) pair    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    
   
    ifstream rawdb_file_handle;
    rawdb_file_handle.open(rawdb_file,ios_base::in|ios_base::binary);

    ofstream eidxdb_file_handle;
    eidxdb_file_handle.open(eidxdb_file,ios_base::out|ios_base::binary);

    stringstream ss;

    string rawdb_row;
    vector<string> rawdb_data;
    string rawdb_row_current;
    string s;

    
    
    //Bloom Filter Parameters
    unsigned int bfidx = 0;
    unsigned int bf_indices[N_HASH];
    unsigned int indices[N_HASH];

    BF = new unsigned char* [N_HASH];
    for(unsigned int k=0;k<N_HASH;++k) {
        BF[k] = new unsigned char [MAX_BF_BIN_SIZE];
        ::memset(BF[k],0x00,MAX_BF_BIN_SIZE);
    }

   

    ::memset(W,0x00,16);
    ::memset(ID,0x00,N_max_id_words*16);
    ::memset(KE,0x00,EVP_MAX_BLOCK_LENGTH);
    ::memset(KE_temp,0x00,16);
    ::memset(XTAG,0x00,N_max_id_words*N_l);
    ::memset(XID,0x00,N_max_id_words*N_l);
    ::memset(WC,0x00,N_max_id_words*16);
    ::memset(R,0x00,N_max_id_words*16);
    ::memset(EC,0x00,N_max_id_words*16);
    ::memset(bhash,0x00,bhash_block_size);

    ::memset(YID_char,0x00,N_max_id_words*2*N_l);
    ::memset(YID,0x00,N_max_id_words*N_l);
    

    ///////////////////////////////////////////////////////////
    
    unsigned char *w_local = W;
    unsigned char *id_local = ID;
    unsigned char *ke_local = KE;
    unsigned char *ke_temp_local = KE_temp;
    
    int16_t *xtag_local = XTAG;
    uint16_t *xid_local = XID;
    

    unsigned char *wc_local = WC;
    unsigned char *r_local = R;
    unsigned char *ec_local = EC;
    unsigned char *yid_char_local = YID_char;

    uint16_t *yid_local = YID;
   
    unsigned char *local_s;
    int n_rows = 0;
    int n_row_ids = 0;

    Sys_Init();

    rawdb_row.clear();
    while(getline(rawdb_file_handle,rawdb_row)){
        rawdb_data.push_back(rawdb_row);
        rawdb_row.clear();
        n_rows++;
    }

    cout << "Number of Keywords: " << n_rows << endl;

    rawdb_file_handle.close();

    auto start_time = std::chrono::high_resolution_clock::now();


    
    for(unsigned int n1=0; n1<n_rows; ++n1) 
    {
        
        ::memset(ID,0x00,N_max_id_words*16);
        ::memset(KE,0x00,EVP_MAX_BLOCK_LENGTH);
        ::memset(KE_temp,0x00,16);
        ::memset(R,0x00,N_max_id_words*16);
        ::memset(EC,0x00,N_max_id_words*16);

        
        rawdb_row_current = rawdb_data.at(n1);

        ss.str(std::string());
        ss << rawdb_row_current;
        std::getline(ss,s,',');             // Read inverted index row


        
        DB_StrToHex8(W,s.data());           //Read the keyword        
        id_local = ID;

        n_row_ids = 0;                                  //For row id count
        while(std::getline(ss,s,',') && !ss.eof()) {
            if(!s.empty()){
                DB_StrToHex8(id_local,s.data());        //Read the id
                
                id_local += 16;
                n_row_ids++;
            }
        }
        ss.clear();
        ss.seekg(0);

        id_local = ID;              //Set local id pointer to the beginning of the array

        
       
        //Generate KE from W and KS
        /* Since each W is 8B output of AES-256 is 16B and KE needs to be 32B to be used as a key, thus we run the loop
        four times to generate a block of 8B four times and concatenate in KE to form a block of 32B */                            
        
        for(int i=0; i<4;i++) {

            ::memset(KE_temp,0x00,16);

            if(!PKCS5_PBKDF2_HMAC_SHA1(KS, strlen(KS),NULL,0,1000,32,KS1))
            {
                printf("Error in key generation\n");
                exit(1);
            }
            // while(!RAND_bytes(iv_ks,sizeof(iv_ks)));


            w_local = W;
            ke_temp_local = KE_temp;
            ke = encrypt(w_local, sizeof(w_local)/sizeof(w_local[0]), aad, sizeof(aad), KS1, iv_ks, ke_temp_local, tag_ks);       
            w_local = W;
            ke_temp_local = KE_temp;

            ::memcpy(KE+(8*i),KE_temp,0x08);

        }       

        //Generate random polynomial wrt XW from Falcon specifications
        TEMPALLOC union {
            uint16_t hm_xw[512];
        } r_xw;
        TEMPALLOC inner_shake256_context sc_xw;

        inner_shake256_init(&sc_xw); 		                            //Initialises the array A to all 0
        inner_shake256_inject(&sc_xw,W, 16);	// injects msg to context sc
        inner_shake256_flip(&sc_xw);
        // Zf(hash_to_point_vartime_check1)(&sc_xw, r_xw.hm_xw, 9);
        Zf(hash_to_point_vartime)(&sc_xw, r_xw.hm_xw, 9);

        uint32_t xw[512];
        
        for(int i=0; i<512; i++){
            xw[i] = (r_xw.hm_xw[i] << 16) % q_l;
        }
        

        //Generating Public key, Private Key and Signature

        // Computing randomness r to pass to keygen algorithm //

        // (append w || counter)
        wc_local = WC;
        for(int nword=0; nword<n_row_ids; ++nword)
        {
            ::memcpy(wc_local,W,16);
            wc_local += 16;
        }
        wc_local = WC;
        
        
        //  PRF to compute r  //    
         unsigned int count_wc = 1;
        unsigned int count_wc_local = 0;
        r_local = R;
        wc_local = WC;
        unsigned char pt[16];
        for(unsigned int nword=0; nword<n_row_ids; nword++)
        {
            count_wc_local = count_wc;
            *(wc_local+0) = count_wc_local & 0xFF;
            count_wc_local >>= 8;
            *(wc_local+1) = count_wc_local & 0xFF;
            count_wc++;


            int kr = encrypt(wc_local, sizeof(wc_local)/sizeof(wc_local[0]), aad, sizeof(aad), KZ1, iv_kz, r_local, tag_kz);
            kr_enc_vec.push_back(kr);
            
            wc_local += 16;
            r_local += 16;
        }
        r_local = R;
        wc_local = WC;
    


        // For Key and Signature generation //
        xid_local = XID;
        yid_local = YID;
        r_local = R;
        id_local = ID;
        xtag_local = XTAG;
        for(unsigned int nword=0; nword<n_row_ids; nword++)
        {
            //  Key Generation  //

            unsigned char seed[16];
            size_t n_keygen;
            size_t tlen_keygen = 90112;
            size_t tlen_sign = 178176;
            unsigned logn_keygen = 9;
            int8_t *f, *g, *F, *G;
            uint16_t *h, *hm, *h2, *hm2, *h_mont;
            int16_t *sig_keygen, *s1_keygen;
            uint8_t *tt_keygen, *tt_sign, *temp_sign;
            inner_shake256_context sc_keygen;
            fpr *expanded_key;             
    
            
            std::memcpy(seed,r_local,16);
            inner_shake256_init(&sc_keygen);
            inner_shake256_inject(&sc_keygen, seed, sizeof(seed));
            inner_shake256_flip(&sc_keygen);
           

            temp_sign = xmalloc(tlen_sign);
            h_mont = (uint16_t *)temp_sign;
            n_keygen = (size_t)1 << logn_keygen;
    
            f = xmalloc(tlen_keygen);
            g = f + n_keygen;
            F = g + n_keygen;
            G = F + n_keygen;
            h = (uint16_t *)(G + n_keygen);
            h2 = h + n_keygen;
	        hm = h2 + n_keygen;
            sig_keygen = (int16_t *)(hm + n_keygen);
            s1_keygen = sig_keygen + n_keygen;
            tt_keygen = (uint8_t *)(s1_keygen + n_keygen);
            tt_sign = (uint8_t *)(s1_keygen + n_keygen);
            if (logn_keygen == 1) {
                tt_keygen += 4;
                tt_sign += 4;
            }
            for (int i = 0; i < 12; i ++) {
                Zf(keygen)(&sc_keygen, f, g, F, G, h, logn_keygen, tt_keygen);
            }
    
            
            // Signature Generation //

            //Public key h in NTT-Montgomery Form
            ::memcpy(h_mont, h, n_keygen * sizeof *h_mont);
	        Zf(to_ntt_monty)(h_mont, logn_keygen);
  
            
            //Expanded private key for NTT operations
            expanded_key = (fpr *)tt_sign;
            tt_sign = (uint8_t *)expanded_key + (8 * logn_keygen + 40) * n_keygen;
            Zf(expand_privkey)(expanded_key, f, g, F, G, logn_keygen, tt_sign);
 

            unsigned char array_xid[16];
            inner_shake256_context sc_xid1;
            size_t tlen_xid1 = 90112;
            int32_t *f_xid1 = xmalloc(tlen_xid1);
            int8_t logn_xid1 = 9;
            size_t n_xid1 = (size_t)1 << logn_xid1;
            uint16_t *hm_xid1  = (uint16_t *) (f_xid1 +6*n_xid1);
            uint32_t *hm_xid_check = f_xid1 +6*n_xid1;



            std::memcpy(array_xid,id_local,16);
            inner_shake256_init(&sc_xid1); 		                            
            inner_shake256_inject(&sc_xid1, array_xid, 16);	
            inner_shake256_flip(&sc_xid1);
            
            Zf(hash_to_point_vartime)(&sc_xid1, hm_xid1, logn_xid1);
            

            
            // Signature Computation --
            //Compute a signature over the provided hashed message sc_xid; the signature value is one short vector.
            //On successful output, the start of the tt_sign buffer contains the s1 vector (int16_t elements).
            //The minimal size (in bytes) of tt_sign is 48*2^logn bytes. 

        
            Zf(sign_tree)(sig_keygen, &sc_xid1, expanded_key, hm_xid1, logn_keygen, tt_sign);
            

            //Signature Verification
            if (!Zf(verify_raw)(hm_xid1, sig_keygen, h_mont, 9, tt_sign)){
                fprintf(stderr, "self signature (dyn) not verified\n");
                exit(EXIT_FAILURE);
		    }   



            size_t n = (size_t)1 << 9;
	        uint16_t tt_s2[512];
            uint16_t tt_s1[512];
            uint16_t tt_s2_backup[512]; 

        
            for (int u = 0; u < n; u ++) {
                uint32_t w;

                w = (uint32_t)sig_keygen[u];
                w += q_l & -(w >> 31);
                tt_s2[u] = (uint16_t)w;
            }

            for(int i = 0; i < 512; i++){
                tt_s2_backup[i] = tt_s2[i];
                yid_local[i] = tt_s2[i];
            }

            mq_NTT(tt_s2, 9);
            mq_poly_montymul_ntt(tt_s2, h_mont, 9); 
            mq_iNTT(tt_s2, 9);

            uint16_t xid_temp[512];
            for(int u=0; u<n; u++) {
                xid_temp[u] = (uint16_t)mq_add(tt_s1[u],tt_s2[u]);  
                xid_local[u] = xid_temp[u];
            }

            mq_poly_sub(xid_local, tt_s1, 9);

            int32_t xtag[512];

            for(int i = 0; i < 512; i++){
                xtag[i] = (xid_local[i] * xw[i])%q_l;
            }


            for(int k=0; k<512; k++){
                xtag_local[k] = floor(xtag[k] * p/p_1);
                xtag_local[k] = xtag_local[k] % p;
            }   

            yid_local += N_l;
            xid_local += N_l;
            xtag_local += N_l;
            
            id_local += 16;
            r_local += 16;
        
        }
        xid_local = XID;
        yid_local = YID;
        r_local = R;
        id_local = ID;
        xtag_local = XTAG;


        
        //AES Encryption of id using KE
        const char* KE1 = reinterpret_cast<const char *> (KE);
        if(!PKCS5_PBKDF2_HMAC_SHA1(KE, strlen(KE),NULL,0,1000,32,KE1))
        {
            printf("Error in key generation\n");
            exit(1);
        }
       
        
        id_local = ID;
        ec_local = EC;
        for(unsigned int nword=0; nword<n_row_ids; nword++)
        {
            kec = encrypt(id_local, sizeof(id_local)/sizeof(id_local[0]), aad, sizeof(aad), KE1, iv_ec, ec_local, tag_ec);
            id_local += sym_block_size;
            ec_local += 16;
        }
        id_local = ID;
        ec_local = EC;



        yid_local = YID;
        yid_char_local = YID_char;
        for(int n=0; n<n_row_ids; n++){   
            for(int k=0; k<N_l; k++){
                    yid_char_local[2*k] = static_cast<unsigned char>(yid_local[k] & 0xFF);          
                    yid_char_local[2*k + 1] = static_cast<unsigned char>((yid_local[k] >> 8) & 0xFF); 
            }
            
            yid_local += N_l;
            yid_char_local += 2*N_l;
        }
        yid_local = YID;
        yid_char_local = YID_char;


        eidxdb_file_handle << DB_HexToStr8(W) << ",";            
        for(int n_eidx=0;n_eidx < n_row_ids;++n_eidx){
            eidxdb_file_handle << DB_HexToStr_N(YID_char+(2*N_l*n_eidx),2*N_l) << DB_HexToStr_N(EC+(16*n_eidx),16) + ",";
        }
       
        eidxdb_file_handle << endl;
        
       
        
        xtag_local = XTAG;
        for(int i=0;i<n_row_ids;++i)
        {

            ::memset(bhash,0x00,bhash_block_size);
            ::memset(bf_indices,0x00,N_HASH);

            unsigned char xtag_char[2*N_l];
            ::memset(xtag_char,0x00,2*N_l);

            unsigned char *xtag_char_local = xtag_char;
            for(int k=0; k<N_l; k++){
                
                xtag_char[2*k] = static_cast<unsigned char>(xtag_local[k] & 0xFF);         
                xtag_char[2*k + 1] = static_cast<unsigned char>((xtag_local[k] >> 8) & 0xFF); 

                    
                
            }
            xtag_char_local = xtag_char;


            BLOOM_HASH(xtag_char,bhash);
        
            for(int j=0;j<N_HASH;++j){
                bf_indices[j] = BFIdxConv(bhash+(64*j),N_BF_BITS);
            }

            BloomFilter_Set(BF, bf_indices);

            xtag_local += N_l;
        }
        
        xtag_local = XTAG;
        
    }

    
    eidxdb_file_handle.close();
    

    ///////////////////////////////////////////////////////////////////////////////////////////////
    
    cout << "TSet SetUp Starting!" << endl << endl;

    TSet_SetUp();

    
    cout << "TSet SetUp Done!" << endl;

    auto stop_time = chrono::high_resolution_clock::now();

    std::cout << "Writing Bloom Filter to disk..." << std::endl;
    BloomFilter_WriteBFtoFile(bloomfilter_file, BF); //Store bloom filter in file


    Sys_Clear();

    
    auto time_elapsed = chrono::duration_cast<chrono::microseconds>(stop_time - start_time).count();
    std::cout << "[*] Setup-time: " << time_elapsed << " micro-seconds" << endl;

    
    return 0;
}   
