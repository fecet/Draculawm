/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx = 1; /* border pixel of windows */
static const unsigned int gappx = 12;       /* gap pixel between windows */
static const unsigned int snap = 32;    /* snap pixel */
static const char *const panel[] = { "xfce4-panel", "Xfce4-panel" }; /* name & cls of panel win */

static const int showbar = 1;           /* 0 means no bar */
static const int topbar = 1;            /* 0 means bottom bar */

#define ICONSIZE 20
#define ICONSPACING 5

static const int focusonwheel = 0;

static const char *fonts[] = { "Sarasa Mono SC Nerd:size=8", "JoyPixels:pixelsize=8:antialias=true:autohint=true"  };
static const char dmenufont[]       = "Sarasa Mono SC Nerd:size=20";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#ffffff";
static const char col_cyan[]        = "#37474F";
static const char col_border[]        = "#42A5F5";
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3] = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_border  },
	[SchemeUrg] = {"#000000", "#F46067", "#F46067"},
	[SchemeTitle] = {"#ffffff", "#222B2E", "#222B2E"},
};

static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
	[SchemeUrg]  = { OPAQUE, baralpha, borderalpha },
	[SchemeTitle]= { OPAQUE, baralpha, borderalpha },
};

/* autostart */
static const char *const autostart[] = {
    "xfsettingsd", "--sm-client-disable", NULL, 
	"xfce4-panel", "--disable-wm-check", NULL,
    "/usr/lib/xfce4/notifyd/xfce4-notifyd", NULL,
    "sh", "/home/rok/scripts/autostart.sh", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "", "", "", "", " ", "五", "六", "\uf537", "\uf684" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Steam",    NULL,       "Friends List", 0,            1,           -1 },
	{ "Steam",    NULL,       "Steam - News", 0,            1,           -1 },
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Android Emulator", NULL,       NULL,       0,            1,           -1 },
	{ "Emulator", NULL,       NULL,       0,            1,           -1 },
	{ "quemu-system-i386", NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
	{ "uTools",   NULL,       NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact = 0.5;  /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;     /* number of clients in master area */
static const int nmaxmaster = 2;        /* maximum number of clients allowed in master area */
static const int resizehints = 0; /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "Tile",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG) \
{MODKEY, KEY, view, {.ui = 1 << TAG}}, {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}}, \
{MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},
//{MODKEY | Mod1Mask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) \
{ \
	.v = (const char *[]) { "/bin/sh", "-c", cmd, NULL } \
}

/* commands */
#define TERM "st"
#define TERMRUN TERM " -e "

static const char *rofiruncmd[] = {"rofi", "-show", "drun", "-modi", "drun", "-show-icons", "-font", "Noto Sans 16", "-icon-theme", "Papirus-Dark", "-run-shell-command", TERMRUN "{cmd}", NULL};
static const char *rofiquitcmd[] = {"rofi", "-show", "p", "-modi", "p:rofi-power-menu", "-font", "NotoSans Nerd Font Regular 16", "-width", "20", "-lines", "4", NULL};

static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
// static const char *termcmd[]  = { "st", NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *browsercmd[]  = { "microsoft-edge-beta", "--proxy-server=127.0.0.1:7890", NULL };
static const char *clipmenu[]  = { "clipmenu", NULL };
static const char *rangercmd[]  = { "alacritty", "-e", "ranger", NULL };
static const char *editorcmd[]  = { "neovide", "--maximized", NULL };
//static const char *transcmd[]  = {"home/rok/scripts/trans.sh", NULL };
static const char *screenshotcmd[] = { "xfce4-screenshooter", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "80x24", NULL };

static Key keys[] = {
//	{ Mod1Mask,            XK_space,                spawn,          {.v = dmenucmd } },
	{ MODKEY,              XK_Return,               spawn,          {.v = termcmd } },
	{ MODKEY,              XK_c,                    spawn,          {.v = browsercmd } },
	{ MODKEY,              XK_v,                    spawn,          {.v = clipmenu } },
	{ MODKEY,              XK_e,                    spawn,          {.v = rangercmd } },
	{ 0,                   XK_Print,                spawn,          {.v = screenshotcmd } },
	{ MODKEY,              XK_i,                    spawn,          {.v = editorcmd } },
	{ MODKEY|ShiftMask,    XK_j,                    rotatestack,    {.i = +1 } },
	{ MODKEY|ShiftMask,    XK_k,                    rotatestack,    {.i = -1 } },
	{ MODKEY,              XK_j,                    focusstack,     {.i = +1 } },
	{ MODKEY,              XK_k,                    focusstack,     {.i = -1 } },
//	{ MODKEY,              XK_h,                    viewtoleft,     {0} },
//	{ MODKEY,              XK_l,                    viewtoright,    {0} },
//	{ MODKEY|ShiftMask,    XK_h,                    tagtoleft,      {0} },
//	{ MODKEY|ShiftMask,    XK_l,                    tagtoright,     {0} },

	{ MODKEY|ShiftMask,    XK_d,                    incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,    XK_i,                    incnmaster,     {.i = -1 } },

	{ MODKEY,              XK_h,                    setmfact,       {.f = -0.05} },
	{ MODKEY,              XK_l,                    setmfact,       {.f = +0.05} },
//	{ MODKEY,              XK_k,                    hidewin,        {0} },
//	{ MODKEY|ShiftMask,    XK_k,                    restorewin,     {0} },
//	{ MODKEY,              XK_o,                    hideotherwins,  {0}},
//	{ MODKEY|ShiftMask,    XK_o,                    restoreotherwins, {0}},
	{ MODKEY|ShiftMask,    XK_Return,               zoom,           {0} },
	{ MODKEY,              XK_Tab,                  view,           {0} },
	{ MODKEY|ShiftMask,    XK_q,                    killclient,     {0} },
//	{ MODKEY,              XK_t,                    setlayout,      {.v = &layouts[0]} },
	{ MODKEY,              XK_m,                    setlayout,      {.v = &layouts[2]} },
	{ MODKEY,              XK_p,                    setlayout,      {0} },
	{ MODKEY|ShiftMask,    XK_space,                togglefloating, {0} },
	{ MODKEY,              XK_apostrophe,           togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,              XK_0,                    view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,    XK_0,                    tag,            {.ui = ~0 } },
//	{ MODKEY,              XK_comma,                focusmon,       {.i = -1 } },
//	{ MODKEY,              XK_period,               focusmon,       {.i = +1 } },
//	{ MODKEY|ShiftMask,    XK_comma,                tagmon,         {.i = -1 } },
//	{ MODKEY|ShiftMask,    XK_period,               tagmon,         {.i = +1 } },
	{ MODKEY,              XK_bracketleft,          focusmon,       {.i = -1   } },
	{ MODKEY,              XK_bracketright,         focusmon,       {.i = +1   } },
	{ MODKEY|ShiftMask,    XK_bracketleft,          tagmon,         {.i = -1   } },
	{ MODKEY|ShiftMask,    XK_bracketright,         tagmon,         {.i = +1   } },
	TAGKEYS(               XK_1,                      0)
	TAGKEYS(               XK_2,                      1)
	TAGKEYS(               XK_3,                      2)
	TAGKEYS(               XK_4,                      3)
	TAGKEYS(               XK_5,                      4)
	TAGKEYS(               XK_6,                      5)
	TAGKEYS(               XK_7,                      6)
	TAGKEYS(               XK_8,                      7)
	TAGKEYS(               XK_9,                      8)
//	{ MODKEY|ControlMask,  XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ClkLtSymbol, 0, Button1, setlayout, {0}},
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	//{ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[1]}},
	//{ClkWinTitle, 0, Button2, zoom, {0}},
	//{ClkStatusText, 0, Button2, spawn, {.v = termcmd}},
	{ClkClientWin, MODKEY, Button1, movemouse, {0}},
	{ClkClientWin, MODKEY, Button2, togglefloating, {0}},
	{ClkClientWin, MODKEY, Button3, resizemouse, {0}},
	{ClkTagBar, 0, Button1, view, {0}},
	{ClkTagBar, 0, Button3, toggleview, {0}},
	//{ClkTagBar, MODKEY, Button1, tag, {0}},
	//{ClkTagBar, MODKEY, Button3, toggletag, {0}},
	{ClkWinTitle, 0, Button1, focuswin, {0}},
};
