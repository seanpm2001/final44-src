h52528
s 00396/00000/00000
d D 5.1 92/07/19 16:28:10 bostic 1 0
c date and time created 92/07/19 16:28:10 by bostic
e
u
U
t
T
I 1















OP("bicpsw", CORE, 0xb9, 1, A_RW,0,0,0,0,0),
OP("bispsw", CORE, 0xb8, 1, A_RW,0,0,0,0,0),
OP("movpsl", CORE, 0xdc, 1, A_WL,0,0,0,0,0),

OP("ldpctx", CORE, 0x06, 0,0,0,0,0,0,0),
OP("svpctx", CORE, 0x07, 0,0,0,0,0,0,0),
OP("mfpr", CORE, 0xdb, 2, A_RL, A_WL,0,0,0,0),
OP("mtpr", CORE, 0xda, 2, A_RL, A_RL,0,0,0,0),

OP("prober", CORE, 0x0c, 3, A_RB, A_RW, A_AB,0,0,0),
OP("probew", CORE, 0x0d, 3, A_RB, A_RW, A_AB,0,0,0),

OP("adawi", CORE, 0x58, 2, A_RW, A_MW,0,0,0,0),

OP("chme", CORE, 0xbd, 1, A_RW,0,0,0,0,0),
OP("chmk", CORE, 0xbc, 1, A_RW,0,0,0,0,0),
OP("chms", CORE, 0xbe, 1, A_RW,0,0,0,0,0),
OP("chmu", CORE, 0xbf, 1, A_RW,0,0,0,0,0),

OP("insque", CORE, 0x0e, 2, A_AB, A_AB,0,0,0,0),
OP("insqhi", NEW, 0x5c, 2, A_AB, A_AQ,0,0,0,0),
OP("insqti", NEW, 0x5d, 2, A_AB, A_AQ,0,0,0,0),

OP("remque", CORE, 0x0f, 2, A_AB, A_WL,0,0,0,0),
OP("remqhi", NEW, 0x5e, 2, A_AQ, A_WL,0,0,0,0),
OP("remqti", NEW, 0x5f, 2, A_AQ, A_WL,0,0,0,0),

OP("bpt", CORE, 0x03, 0,0,0,0,0,0,0),

OP("escd", CORE, 0xfd, 0,0,0,0,0,0,0),
OP("esce", CORE, 0xfe, 0,0,0,0,0,0,0),
OP("escf", CORE, 0xff, 0,0,0,0,0,0,0),
OP("xfc", CORE, 0xfc, 0,0,0,0,0,0,0),

OP("bugw", ESCF, 0xfe, 1, A_RW,0,0,0,0,0),
OP("bugl", ESCF, 0xfd, 1, A_RL,0,0,0,0,0),

OP("nop", CORE, 0x01, 0,0,0,0,0,0,0),
OP("halt", CORE, 0x00, 0,0,0,0,0,0,0),

OP("crc", CORE, 0x0b, 4, A_AB, A_RL, A_RW, A_AB,0,0),

OP("movp", CORE, 0x34, 3, A_RW, A_AB, A_AB,0,0,0),
OP("cvtlp", CORE, 0xf9, 3, A_RL, A_RW, A_AB,0,0,0),
OP("cvtpl", CORE, 0x36, 3, A_RW, A_AB, A_WL,0,0,0),
OP("cvttp", CORE, 0x26, 5, A_RW, A_AB, A_AB, A_RW, A_AB,0),
OP("cvtpt", CORE, 0x24, 5, A_RW, A_AB, A_AB, A_RW, A_AB,0),
OP("cvtps", CORE, 0x08, 4, A_RW, A_AB, A_RW, A_AB,0,0),
OP("cvtsp", CORE, 0x09, 4, A_RW, A_AB, A_RW, A_AB,0,0),

OP("addp4", CORE, 0x20, 4, A_RW, A_AB, A_RW, A_AB,0,0),
OP("addp6", CORE, 0x21, 6, A_RW, A_AB, A_RW, A_AB, A_RW, A_AB),
OP("subp4", CORE, 0x22, 4, A_RW, A_AB, A_RW, A_AB,0,0),
OP("subp6", CORE, 0x23, 6, A_RW, A_AB, A_RW, A_AB, A_RW, A_AB),
OP("mulp", CORE, 0x25, 6, A_RW, A_AB, A_RW, A_AB, A_RW, A_AB),
OP("divp", CORE, 0x27, 6, A_RW, A_AB, A_RW, A_AB, A_RW, A_AB),
OP("cmpp3", CORE, 0x35, 3, A_RW, A_AB, A_AB,0,0,0),
OP("cmpp4", CORE, 0x37, 4, A_RW, A_AB, A_RW, A_AB,0,0),

OP("insv", CORE, 0xf0, 4, A_RL, A_RL, A_RB, A_WB,0,0),
OP("extv", CORE, 0xee, 4, A_RL, A_RB, A_RB, A_WL,0,0),
OP("extzv", CORE, 0xef, 4, A_RL, A_RB, A_RB, A_WL,0,0),
OP("cmpv", CORE, 0xec, 4, A_RL, A_RB, A_RB, A_RL,0,0),
OP("cmpzv", CORE, 0xed, 4, A_RL, A_RB, A_RB, A_RL,0,0),

OP("editpc", CORE, 0x38, 4, A_RW, A_AB, A_AB, A_AB,0,0),
OP("locc", CORE, 0x3a, 3, A_RB, A_RW, A_AB,0,0,0),
OP("matchc", CORE, 0x39, 4, A_RW, A_AB, A_RW, A_AB,0,0),
OP("skpc", CORE, 0x3b, 3, A_RB, A_RW, A_AB,0,0,0),
OP("scanc", CORE, 0x2a, 4, A_RW, A_AB, A_AB, A_RB,0,0),
OP("spanc", CORE, 0x2b, 4, A_RW, A_AB, A_AB, A_RB,0,0),
OP("movc3", CORE, 0x28, 3, A_RW, A_AB, A_AB,0,0,0),
OP("movc5", CORE, 0x2c, 5, A_RW, A_AB, A_RB, A_RW, A_AB,0),
OP("movtc", CORE, 0x2e, 6, A_RW, A_AB, A_RB, A_AB, A_RW, A_AB),
OP("movtuc", CORE, 0x2f, 6, A_RW, A_AB, A_RB, A_AB, A_RW, A_AB),
OP("cmpc3", CORE, 0x29, 3, A_RW, A_AB, A_AB,0,0,0),
OP("cmpc5", CORE, 0x2d, 5, A_RW, A_AB, A_RB, A_RW, A_AB,0),

OP("adwc", CORE, 0xd8, 2, A_RL, A_ML,0,0,0,0),
OP("sbwc", CORE, 0xd9, 2, A_RL, A_ML,0,0,0,0),

OP("rotl", CORE, 0x9c, 3, A_RB, A_RL, A_WL,0,0,0),
OP("ashl", CORE, 0x78, 3, A_RB, A_RL, A_WL,0,0,0),
OP("ashp", CORE, 0xf8, 6, A_RB, A_RW, A_AB, A_RB, A_RW, A_AB),
OP("ashq", CORE, 0x79, 3, A_RB, A_RQ, A_WQ,0,0,0),

OP("jmp", CORE, 0x17, 1, A_AB,0,0,0,0,0),
OP("brb", CORE, 0x11, 1, A_BB,0,0,0,0,0),
OP("brw", CORE, 0x31, 1, A_BW,0,0,0,0,0),

OP("bitb", CORE, 0x93, 2, A_RB, A_RB,0,0,0,0),
OP("bitw", CORE, 0xb3, 2, A_RW, A_RW,0,0,0,0),
OP("bitl", CORE, 0xd3, 2, A_RL, A_RL,0,0,0,0),

OP("bbc", CORE, 0xe1, 3, A_RL, A_RB, A_BB,0,0,0),
OP("bbcc", CORE, 0xe5, 3, A_RL, A_RB, A_BB,0,0,0),
OP("bbcci", CORE, 0xe7, 3, A_RL, A_RB, A_BB,0,0,0),
OP("bbcs", CORE, 0xe3, 3, A_RL, A_RB, A_BB,0,0,0),
OP("bbs", CORE, 0xe0, 3, A_RL, A_RB, A_BB,0,0,0),
OP("bbsc", CORE, 0xe4, 3, A_RL, A_RB, A_BB,0,0,0),
OP("bbss", CORE, 0xe2, 3, A_RL, A_RB, A_BB,0,0,0),
OP("bbssi", CORE, 0xe6, 3, A_RL, A_RB, A_BB,0,0,0),
OP("bcc", CORE, 0x1e, 1, A_BB,0,0,0,0,0),
OP("bcs", CORE, 0x1f, 1, A_BB,0,0,0,0,0),
OP("blbs", CORE, 0xe8, 2, A_RL, A_BB,0,0,0,0),
OP("blbc", CORE, 0xe9, 2, A_RL, A_BB,0,0,0,0),

OP("beql", CORE, 0x13, 1, A_BB,0,0,0,0,0),
OP("beqlu", CORE, 0x13, 1, A_BB,0,0,0,0,0),
OP("bgeq", CORE, 0x18, 1, A_BB,0,0,0,0,0),
OP("bgequ", CORE, 0x1e, 1, A_BB,0,0,0,0,0),
OP("bgtr", CORE, 0x14, 1, A_BB,0,0,0,0,0),
OP("bgtru", CORE, 0x1a, 1, A_BB,0,0,0,0,0),
OP("bleq", CORE, 0x15, 1, A_BB,0,0,0,0,0),
OP("blequ", CORE, 0x1b, 1, A_BB,0,0,0,0,0),
OP("blss", CORE, 0x19, 1, A_BB,0,0,0,0,0),
OP("blssu", CORE, 0x1f, 1, A_BB,0,0,0,0,0),
OP("bneq", CORE, 0x12, 1, A_BB,0,0,0,0,0),
OP("bnequ", CORE, 0x12, 1, A_BB,0,0,0,0,0),
OP("bvc", CORE, 0x1c, 1, A_BB,0,0,0,0,0),
OP("bvs", CORE, 0x1d, 1, A_BB,0,0,0,0,0),

OP("caseb", CORE, 0x8f, 3, A_RB, A_RB, A_RB,0,0,0),
OP("casew", CORE, 0xaf, 3, A_RW, A_RW, A_RW,0,0,0),
OP("casel", CORE, 0xcf, 3, A_RL, A_RL, A_RL,0,0,0),

OP("clrb", CORE, 0x94, 1, A_WB,0,0,0,0,0),
OP("clrw", CORE, 0xb4, 1, A_WW,0,0,0,0,0),
OP("clrl", CORE, 0xd4, 1, A_WL,0,0,0,0,0),
OP("clrq", CORE, 0x7c, 1, A_WQ,0,0,0,0,0),
OP("clro", ESCD, 0x7c, 1, A_WO,0,0,0,0,0),
OP("clrf", CORE, 0xd4, 1, A_WF,0,0,0,0,0),
OP("clrd", CORE, 0x7c, 1, A_WD,0,0,0,0,0),
OP("clrg", CORE, 0x7c, 1, A_WD,0,0,0,0,0),
OP("clrh", ESCD, 0x7c, 1, A_WH,0,0,0,0,0),

OP("cvtbw", CORE, 0x99, 2, A_RB, A_WW,0,0,0,0),
OP("cvtbl", CORE, 0x98, 2, A_RB, A_WL,0,0,0,0),
OP("cvtbf", CORE, 0x4c, 2, A_RB, A_WF,0,0,0,0),
OP("cvtbg", ESCD, 0x4c, 2, A_RB, A_WG,0,0,0,0),
OP("cvtbd", CORE, 0x6c, 2, A_RB, A_WD,0,0,0,0),
OP("cvtbh", ESCD, 0x6c, 2, A_RB, A_WH,0,0,0,0),

OP("cvtwb", CORE, 0x33, 2, A_RW, A_WB,0,0,0,0),
OP("cvtwl", CORE, 0x32, 2, A_RW, A_WL,0,0,0,0),
OP("cvtwf", CORE, 0x4d, 2, A_RW, A_WF,0,0,0,0),
OP("cvtwg", ESCD, 0x4d, 2, A_RW, A_WG,0,0,0,0),
OP("cvtwd", CORE, 0x6d, 2, A_RW, A_WD,0,0,0,0),
OP("cvtwh", ESCD, 0x6d, 2, A_RW, A_WH,0,0,0,0),

OP("cvtlb", CORE, 0xf6, 2, A_RL, A_WB,0,0,0,0),
OP("cvtlw", CORE, 0xf7, 2, A_RL, A_WW,0,0,0,0),
OP("cvtlf", CORE, 0x4e, 2, A_RL, A_WF,0,0,0,0),
OP("cvtlg", ESCD, 0x4e, 2, A_RL, A_WG,0,0,0,0),
OP("cvtld", CORE, 0x6e, 2, A_RL, A_WD,0,0,0,0),
OP("cvtlh", ESCD, 0x6e, 2, A_RL, A_WH,0,0,0,0),

OP("cvtfb", CORE, 0x48, 2, A_RF, A_WB,0,0,0,0),
OP("cvtfw", CORE, 0x49, 2, A_RF, A_WW,0,0,0,0),
OP("cvtfl", CORE, 0x4a, 2, A_RF, A_WL,0,0,0,0),
OP("cvtrfl", CORE, 0x4b, 2, A_RF, A_WL,0,0,0,0),
OP("cvtfd", CORE, 0x56, 2, A_RF, A_WD,0,0,0,0),
OP("cvtfg", ESCD, 0x99, 2, A_RF, A_WG,0,0,0,0),
OP("cvtfh", ESCD, 0x98, 2, A_RF, A_WH,0,0,0,0),

OP("cvtgb", ESCD, 0x48, 2, A_RG, A_WB,0,0,0,0),
OP("cvtgw", ESCD, 0x49, 2, A_RG, A_WW,0,0,0,0),
OP("cvtgl", ESCD, 0x4a, 2, A_RG, A_WL,0,0,0,0),
OP("cvtrgl", ESCD, 0x4b, 2, A_RG, A_WL,0,0,0,0),
OP("cvtgf", ESCD, 0x33, 2, A_RG, A_WF,0,0,0,0),
OP("cvtgh", ESCD, 0x56, 2, A_RG, A_WH,0,0,0,0),

OP("cvtdb", CORE, 0x68, 2, A_RD, A_WB,0,0,0,0),
OP("cvtdw", CORE, 0x69, 2, A_RD, A_WW,0,0,0,0),
OP("cvtdl", CORE, 0x6a, 2, A_RD, A_WL,0,0,0,0),
OP("cvtrdl", CORE, 0x6b, 2, A_RD, A_WL,0,0,0,0),
OP("cvtdf", CORE, 0x76, 2, A_RD, A_WF,0,0,0,0),
OP("cvtdh", ESCD, 0x32, 2, A_RD, A_WH,0,0,0,0),

OP("cvthb", ESCD, 0x68, 2, A_RH, A_WB,0,0,0,0),
OP("cvthw", ESCD, 0x69, 2, A_RH, A_WW,0,0,0,0),
OP("cvthl", ESCD, 0x6a, 2, A_RH, A_WL,0,0,0,0),
OP("cvtrhl", ESCD, 0x6b, 2, A_RH, A_WL,0,0,0,0),
OP("cvthf", ESCD, 0xf6, 2, A_RH, A_WF,0,0,0,0),
OP("cvthd", ESCD, 0xf7, 2, A_RH, A_WD,0,0,0,0),
OP("cvthg", ESCD, 0x76, 2, A_RH, A_WG,0,0,0,0),

OP("ffc", CORE, 0xeb, 4, A_RL, A_RB, A_RB, A_WL,0,0),
OP("ffs", CORE, 0xea, 4, A_RL, A_RB, A_RB, A_WL,0,0),

OP("index", CORE, 0x0a, 6, A_RL, A_RL, A_RL, A_RL, A_RL, A_WL),

OP("rei", CORE, 0x02, 0,0,0,0,0,0,0),

OP("callg", CORE, 0xfa, 2, A_AB, A_AB,0,0,0,0),
OP("calls", CORE, 0xfb, 2, A_RL, A_AB,0,0,0,0),
OP("ret", CORE, 0x04, 0,0,0,0,0,0,0),

OP("jsb", CORE, 0x16, 1, A_AB,0,0,0,0,0),
OP("bsbb", CORE, 0x10, 1, A_BB,0,0,0,0,0),
OP("bsbw", CORE, 0x30, 1, A_BW,0,0,0,0,0),
OP("rsb", CORE, 0x05, 0,0,0,0,0,0,0),

OP("pushr", CORE, 0xbb, 1, A_RW,0,0,0,0,0),
OP("popr", CORE, 0xba, 1, A_RW,0,0,0,0,0),

OP("mnegb", CORE, 0x8e, 2, A_RB, A_WB,0,0,0,0),
OP("mnegw", CORE, 0xae, 2, A_RW, A_WL,0,0,0,0),
OP("mnegl", CORE, 0xce, 2, A_RL, A_WL,0,0,0,0),
OP("mnegf", CORE, 0x52, 2, A_RF, A_WF,0,0,0,0),
OP("mnegg", ESCD, 0x52, 2, A_RG, A_WG,0,0,0,0),
OP("mnegd", CORE, 0x72, 2, A_RD, A_WD,0,0,0,0),
OP("mnegh", ESCD, 0x72, 2, A_RH, A_WH,0,0,0,0),

OP("movab", CORE, 0x9e, 2, A_AB, A_WL,0,0,0,0),
OP("movaw", CORE, 0x3e, 2, A_AW, A_WL,0,0,0,0),
OP("moval", CORE, 0xde, 2, A_AL, A_WL,0,0,0,0),
OP("movaq", CORE, 0x7e, 2, A_AQ, A_WL,0,0,0,0),
OP("movao", ESCD, 0x7e, 2, A_AO, A_WL,0,0,0,0),
OP("movaf", CORE, 0xde, 2, A_AF, A_WL,0,0,0,0),
OP("movad", CORE, 0x7e, 2, A_AD, A_WL,0,0,0,0),
OP("movag", CORE, 0x7e, 2, A_AG, A_WL,0,0,0,0),
OP("movah", ESCD, 0x7e, 2, A_AH, A_WL,0,0,0,0),

OP("movb", CORE, 0x90, 2, A_RB, A_WB,0,0,0,0),
OP("movw", CORE, 0xb0, 2, A_RW, A_WW,0,0,0,0),
OP("movl", CORE, 0xd0, 2, A_RL, A_WL,0,0,0,0),
OP("movf", CORE, 0x50, 2, A_RF, A_WF,0,0,0,0),
OP("movq", CORE, 0x7d, 2, A_RQ, A_WQ,0,0,0,0),
OP("movo", ESCD, 0x7d, 2, A_RO, A_WO,0,0,0,0),
OP("movg", ESCD, 0x50, 2, A_RG, A_WG,0,0,0,0),
OP("movd", CORE, 0x70, 2, A_RD, A_WD,0,0,0,0),
OP("movh", ESCD, 0x70, 2, A_RH, A_WH,0,0,0,0),

OP("movzbl", CORE, 0x9a, 2, A_RB, A_WL,0,0,0,0),
OP("movzbw", CORE, 0x9b, 2, A_RB, A_WW,0,0,0,0),
OP("movzwl", CORE, 0x3c, 2, A_RW, A_WL,0,0,0,0),

OP("pushl", CORE, 0xdd, 1, A_RL,0,0,0,0,0),
OP("pushab", CORE, 0x9f, 1, A_AB,0,0,0,0,0),
OP("pushaw", CORE, 0x3f, 1, A_AW,0,0,0,0,0),
OP("pushal", CORE, 0xdf, 1, A_AL,0,0,0,0,0),
OP("pushaq", CORE, 0x7f, 1, A_AQ,0,0,0,0,0),
OP("pushao", ESCD, 0x7f, 1, A_AO,0,0,0,0,0),
OP("pushaf", CORE, 0xdf, 1, A_AF,0,0,0,0,0),
OP("pushad", CORE, 0x7f, 1, A_AD,0,0,0,0,0),
OP("pushag", CORE, 0x7f, 1, A_AG,0,0,0,0,0),
OP("pushah", ESCD, 0x7f, 1, A_AH,0,0,0,0,0),

OP("acbb", CORE, 0x9d, 4, A_RB, A_RB, A_MB, A_BW,0,0),
OP("acbw", CORE, 0x3d, 4, A_RW, A_RW, A_MW, A_BW,0,0),
OP("acbl", CORE, 0xf1, 4, A_RL, A_RL, A_ML, A_BW,0,0),
OP("acbf", CORE, 0x4f, 4, A_RF, A_RF, A_MF, A_BW,0,0),
OP("acbg", ESCD, 0x4f, 4, A_RG, A_RG, A_MG, A_BW,0,0),
OP("acbd", CORE, 0x6f, 4, A_RD, A_RD, A_MD, A_BW,0,0),
OP("acbh", ESCD, 0x6f, 4, A_RH, A_RH, A_MH, A_BW,0,0),

OP("aobleq", CORE, 0xf3, 3, A_RL, A_ML, A_BB,0,0,0),
OP("aoblss", CORE, 0xf2, 3, A_RL, A_ML, A_BB,0,0,0),
OP("sobgeq", CORE, 0xf4, 2, A_ML, A_BB,0,0,0,0),
OP("sobgtr", CORE, 0xf5, 2, A_ML, A_BB,0,0,0,0),

OP("addb2", CORE, 0x80, 2, A_RB, A_MB,0,0,0,0),
OP("addb3", CORE, 0x81, 3, A_RB, A_RB, A_WB,0,0,0),
OP("addw2", CORE, 0xa0, 2, A_RW, A_MW,0,0,0,0),
OP("addw3", CORE, 0xa1, 3, A_RW, A_RW, A_WW,0,0,0),
OP("addl2", CORE, 0xc0, 2, A_RL, A_ML,0,0,0,0),
OP("addl3", CORE, 0xc1, 3, A_RL, A_RL, A_WL,0,0,0),
OP("addf2", CORE, 0x40, 2, A_RF, A_MF,0,0,0,0),
OP("addf3", CORE, 0x41, 3, A_RF, A_RF, A_WF,0,0,0),
OP("addg2", ESCD, 0x40, 2, A_RG, A_MG,0,0,0,0),
OP("addg3", ESCD, 0x41, 3, A_RG, A_RG, A_WG,0,0,0),
OP("addd2", CORE, 0x60, 2, A_RD, A_MD,0,0,0,0),
OP("addd3", CORE, 0x61, 3, A_RD, A_RD, A_WD,0,0,0),
OP("addh2", ESCD, 0x60, 2, A_RH, A_MH,0,0,0,0),
OP("addh3", ESCD, 0x61, 3, A_RH, A_RH, A_WH,0,0,0),

OP("subb2", CORE, 0x82, 2, A_RB, A_MB,0,0,0,0),
OP("subb3", CORE, 0x83, 3, A_RB, A_RB, A_WB,0,0,0),
OP("subw2", CORE, 0xa2, 2, A_RW, A_MW,0,0,0,0),
OP("subw3", CORE, 0xa3, 3, A_RW, A_RW, A_WW,0,0,0),
OP("subl2", CORE, 0xc2, 2, A_RL, A_ML,0,0,0,0),
OP("subl3", CORE, 0xc3, 3, A_RL, A_RL, A_WL,0,0,0),
OP("subf2", CORE, 0x42, 2, A_RF, A_MF,0,0,0,0),
OP("subf3", CORE, 0x43, 3, A_RF, A_RF, A_WF,0,0,0),
OP("subg2", ESCD, 0x42, 2, A_RG, A_MG,0,0,0,0),
OP("subg3", ESCD, 0x43, 3, A_RG, A_RG, A_WG,0,0,0),
OP("subd2", CORE, 0x62, 2, A_RD, A_MD,0,0,0,0),
OP("subd3", CORE, 0x63, 3, A_RD, A_RD, A_WD,0,0,0),
OP("subh2", ESCD, 0x62, 2, A_RH, A_MH,0,0,0,0),
OP("subh3", ESCD, 0x63, 3, A_RH, A_RH, A_WH,0,0,0),

OP("mulb2", CORE, 0x84, 2, A_RB, A_MB,0,0,0,0),
OP("mulb3", CORE, 0x85, 3, A_RB, A_RB, A_WB,0,0,0),
OP("mulw2", CORE, 0xa4, 2, A_RW, A_MW,0,0,0,0),
OP("mulw3", CORE, 0xa5, 3, A_RW, A_RW, A_WW,0,0,0),
OP("mull2", CORE, 0xc4, 2, A_RL, A_ML,0,0,0,0),
OP("mull3", CORE, 0xc5, 3, A_RL, A_RL, A_WL,0,0,0),
OP("mulf2", CORE, 0x44, 2, A_RF, A_MF,0,0,0,0),
OP("mulf3", CORE, 0x45, 3, A_RF, A_RF, A_WF,0,0,0),
OP("mulg2", ESCD, 0x44, 2, A_RG, A_MG,0,0,0,0),
OP("mulg3", ESCD, 0x45, 3, A_RG, A_RG, A_WG,0,0,0),
OP("muld2", CORE, 0x64, 2, A_RD, A_MD,0,0,0,0),
OP("muld3", CORE, 0x65, 3, A_RD, A_RD, A_WD,0,0,0),
OP("mulh2", ESCD, 0x64, 2, A_RH, A_MH,0,0,0,0),
OP("mulh3", ESCD, 0x65, 3, A_RH, A_RH, A_WH,0,0,0),

OP("divb2", CORE, 0x86, 2, A_RB, A_MB,0,0,0,0),
OP("divb3", CORE, 0x87, 3, A_RB, A_RB, A_WB,0,0,0),
OP("divw2", CORE, 0xa6, 2, A_RW, A_MW,0,0,0,0),
OP("divw3", CORE, 0xa7, 3, A_RW, A_RW, A_WW,0,0,0),
OP("divl2", CORE, 0xc6, 2, A_RL, A_ML,0,0,0,0),
OP("divl3", CORE, 0xc7, 3, A_RL, A_RL, A_WL,0,0,0),
OP("divf2", CORE, 0x46, 2, A_RF, A_MF,0,0,0,0),
OP("divf3", CORE, 0x47, 3, A_RF, A_RF, A_WF,0,0,0),
OP("divg2", ESCD, 0x46, 2, A_RG, A_MG,0,0,0,0),
OP("divg3", ESCD, 0x47, 3, A_RG, A_RG, A_WG,0,0,0),
OP("divd2", CORE, 0x66, 2, A_RD, A_MD,0,0,0,0),
OP("divd3", CORE, 0x67, 3, A_RD, A_RD, A_RD,0,0,0),
OP("divh2", ESCD, 0x66, 2, A_RH, A_MH,0,0,0,0),
OP("divh3", ESCD, 0x67, 3, A_RH, A_RH, A_RH,0,0,0),

OP("incb", CORE, 0x96, 1, A_MB,0,0,0,0,0),
OP("incw", CORE, 0xb6, 1, A_MW,0,0,0,0,0),
OP("incl", CORE, 0xd6, 1, A_ML,0,0,0,0,0),
OP("decb", CORE, 0x97, 1, A_MB,0,0,0,0,0),
OP("decl", CORE, 0xd7, 1, A_ML,0,0,0,0,0),
OP("decw", CORE, 0xb7, 1, A_MW,0,0,0,0,0),

OP("cmpb", CORE, 0x91, 2, A_RB, A_RB,0,0,0,0),
OP("cmpw", CORE, 0xb1, 2, A_RW, A_RW,0,0,0,0),
OP("cmpl", CORE, 0xd1, 2, A_RL, A_RL,0,0,0,0),
OP("cmpf", CORE, 0x51, 2, A_RF, A_RF,0,0,0,0),
OP("cmpg", ESCD, 0x51, 2, A_RG, A_RG,0,0,0,0),
OP("cmpd", CORE, 0x71, 2, A_RD, A_RD,0,0,0,0),
OP("cmph", ESCD, 0x71, 2, A_RH, A_RH,0,0,0,0),

OP("tstb", CORE, 0x95, 1, A_RB,0,0,0,0,0),
OP("tstw", CORE, 0xb5, 1, A_RW,0,0,0,0,0),
OP("tstl", CORE, 0xd5, 1, A_RL,0,0,0,0,0),
OP("tstf", CORE, 0x53, 1, A_RF,0,0,0,0,0),
OP("tstg", ESCD, 0x53, 1, A_RG,0,0,0,0,0),
OP("tstd", CORE, 0x73, 1, A_RD,0,0,0,0,0),
OP("tsth", ESCD, 0x73, 1, A_RH,0,0,0,0,0),

OP("polyf", CORE, 0x55, 3, A_RF, A_RW, A_AB,0,0,0),
OP("polyg", ESCD, 0x55, 3, A_RG, A_RW, A_AB,0,0,0),
OP("polyd", CORE, 0x75, 3, A_RD, A_RW, A_AB,0,0,0),
OP("polyh", ESCD, 0x75, 3, A_RH, A_RW, A_AB,0,0,0),

OP("emul", CORE, 0x7a, 4, A_RL, A_RL, A_RL, A_WQ,0,0),
OP("ediv", CORE, 0x7b, 4, A_RL, A_RQ, A_WL, A_WL,0,0),
OP("emodf", CORE, 0x54, 5, A_RF, A_RB, A_RF, A_WL, A_WF,0),
OP("emodd", CORE, 0x74, 5, A_RD, A_RB, A_RD, A_WL, A_WD,0),
OP("emodg", ESCD, 0x54, 5, A_RG, A_RW, A_RG, A_WL, A_WG,0),
OP("emodh", ESCD, 0x74, 5, A_RH, A_RW, A_RH, A_WL, A_WH,0),

OP("mcomb", CORE, 0x92, 2, A_RB, A_WB,0,0,0,0),
OP("mcomw", CORE, 0xb2, 2, A_RW, A_WW,0,0,0,0),
OP("mcoml", CORE, 0xd2, 2, A_RL, A_WL,0,0,0,0),

OP("bicl2", CORE, 0xca, 2, A_RL, A_ML,0,0,0,0),
OP("bicl3", CORE, 0xcb, 3, A_RL, A_RL, A_WL,0,0,0),
OP("bicw2", CORE, 0xaa, 2, A_RW, A_MW,0,0,0,0),
OP("bicw3", CORE, 0xab, 3, A_RW, A_RW, A_WW,0,0,0),
OP("bicb2", CORE, 0x8a, 2, A_RB, A_MB,0,0,0,0),
OP("bicb3", CORE, 0x8b, 3, A_RB, A_RB, A_WB,0,0,0),

OP("bisb2", CORE, 0x88, 2, A_RB, A_MB,0,0,0,0),
OP("bisb3", CORE, 0x89, 3, A_RB, A_RB, A_WB,0,0,0),
OP("bisw2", CORE, 0xa8, 2, A_RW, A_MW,0,0,0,0),
OP("bisw3", CORE, 0xa9, 3, A_RW, A_RW, A_WW,0,0,0),
OP("bisl2", CORE, 0xc8, 2, A_RL, A_ML,0,0,0,0),
OP("bisl3", CORE, 0xc9, 3, A_RL, A_RL, A_WL,0,0,0),

OP("xorb2", CORE, 0x8c, 2, A_RB, A_MB,0,0,0,0),
OP("xorb3", CORE, 0x8d, 3, A_RB, A_RB, A_WB,0,0,0),
OP("xorw2", CORE, 0xac, 2, A_RW, A_MW,0,0,0,0),
OP("xorw3", CORE, 0xad, 3, A_RW, A_RW, A_WW,0,0,0),
OP("xorl2", CORE, 0xcc, 2, A_RL, A_ML,0,0,0,0),
OP("xorl3", CORE, 0xcd, 3, A_RL, A_RL, A_WL,0,0,0),
E 1
