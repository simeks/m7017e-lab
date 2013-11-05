require "tundra.syntax.glob"
native = require "tundra.native"

local win32_config = {
	Env = {
		GTKMM_BASEPATH = native.getenv("GTKMM_BASEPATH"),

		CXXOPTS = {
			"/MD", "/W4", "/EHsc", "/D_CRT_SECURE_NO_WARNINGS",
			"/wd4250", -- C4250: "inherits '...' via dominance"
			"/wd4127", -- C4127: conditional expression is constant
			"/wd4244", -- C4244: '=' : conversion from 'Glib::IOCondition' to 'gushort', possible loss of data"
			{ "/Od"; Config = "*-*-debug" },
			{ "/O2"; Config = {"*-*-release", "*-*-production"} },
		},
		GENERATE_PDB = {
			{ "0"; Config = "*-vs2012-release" },
			{ "1"; Config = { "*-vs2012-debug", "*-vs2012-production" } },
		}
	},
}

Build {
	Configs = {
		Config {
			Name = "macosx-gcc",
			DefaultOnHost = "macosx",
			Tools = { "gcc-osx" },
		},
		Config {
			Name = 'win32-vs2012',
			Tools = { { "msvc-vs2012"; TargetArch = "x86" }, },
			DefaultOnHost = "windows",
			Inherit = win32_config,
		},
        Config {
                Name = "linux-gcc",
                DefaultOnHost = "linux",
                Tools = { "gcc" },
        },
	},

	IdeGenerationHints = {
		Msvc = {
			-- Remap config names to MSVC platform names (affects things like header scanning & debugging)
			PlatformMappings = {
				['win64-vs2012'] = 'x64',
				['win32-vs2012'] = 'Win32',
			},
			-- Remap variant names to MSVC friendly names
			VariantMappings = {
				['release']    = 'Release',
				['debug']      = 'Debug',
				['production'] = 'Production',
			},
		},

		-- Override solutions to generate and what units to put where.
		MsvcSolutions = {
			['m7017e-lab.sln'] = {},          -- receives all the units due to empty set
		},
	},

	Units = function ()
		Program {
			Name = "Main",
			Sources = {
				FGlob {
					Dir = "src",
					Extensions = { ".cpp", ".h", ".inl" },
					Filters = {
						{ Pattern = "win32"; Config = "win32-*-*"; },
					},
				},
			},
			Env = {
				CPPPATH = { 
					"$(GTKMM_BASEPATH)/include",
					"$(GTKMM_BASEPATH)/include/freetype2",
					"$(GTKMM_BASEPATH)/include/glib-2.0",
					"$(GTKMM_BASEPATH)/include/cairo",
					"$(GTKMM_BASEPATH)/include/pango-1.0",
					"$(GTKMM_BASEPATH)/include/atk-1.0",
					"$(GTKMM_BASEPATH)/include/gdk-pixbuf-2.0",
					"$(GTKMM_BASEPATH)/include/gtk-2.0",
					"$(GTKMM_BASEPATH)/include/libxml2",
					"$(GTKMM_BASEPATH)/include/libglade-2.0",
					"$(GTKMM_BASEPATH)/include/sigc++-2.0",
					"$(GTKMM_BASEPATH)/include/glibmm-2.4",
					"$(GTKMM_BASEPATH)/include/giomm-2.4",
					"$(GTKMM_BASEPATH)/include/cairomm-1.0",
					"$(GTKMM_BASEPATH)/include/pangomm-1.4",
					"$(GTKMM_BASEPATH)/include/atkmm-1.6",
					"$(GTKMM_BASEPATH)/include/gdkmm-2.4",
					"$(GTKMM_BASEPATH)/include/gtkmm-2.4",
					"$(GTKMM_BASEPATH)/include/libxml++-2.6",
					"$(GTKMM_BASEPATH)/include/libglademm-2.4",
					"$(GTKMM_BASEPATH)/lib/glib-2.0/include",
					"$(GTKMM_BASEPATH)/lib/gtk-2.0/include",
					"$(GTKMM_BASEPATH)/lib/sigc++-2.0/include",
					"$(GTKMM_BASEPATH)/lib/glibmm-2.4/include",
					"$(GTKMM_BASEPATH)/lib/giomm-2.4/include",
					"$(GTKMM_BASEPATH)/lib/cairomm-1.0/include",
					"$(GTKMM_BASEPATH)/lib/pangomm-1.4/include",
					"$(GTKMM_BASEPATH)/lib/atkmm-1.6/include",
					"$(GTKMM_BASEPATH)/lib/gdkmm-2.4/include",
					"$(GTKMM_BASEPATH)/lib/gtkmm-2.4/include",
					"$(GTKMM_BASEPATH)/lib/libxml++-2.6/include",
					"$(GTKMM_BASEPATH)/lib/libglademm-2.4/include",
				},
				LIBPATH = {
					"$(GTKMM_BASEPATH)/lib",
				},
				LIBS = {
					"glademm-vc100-d-2_4.lib",
					"xml++-vc100-d-2_6.lib",
					"gtkmm-vc100-d-2_4.lib",
					"gdkmm-vc100-d-2_4.lib",
					"atkmm-vc100-d-1_6.lib",
					"pangomm-vc100-d-1_4.lib",
					"cairomm-vc100-d-1_0.lib",
					"giomm-vc100-d-2_4.lib",
					"glibmm-vc100-d-2_4.lib",
					"sigc-vc100-d-2_0.lib",
					"glade-2.0.lib",
					"libxml2.lib",
					"gtk-win32-2.0.lib",
					"gdk-win32-2.0.lib",
					"gdk_pixbuf-2.0.lib",
					"atk-1.0.lib",
					"pangowin32-1.0.lib",
					"pangocairo-1.0.lib",
					"pango-1.0.lib",
					"cairo.lib",
					"gio-2.0.lib",
					"gobject-2.0.lib",
					"gmodule-2.0.lib",
					"glib-2.0.lib",
					"intl.lib",
				},
			},
			Libs = { { "kernel32.lib", "user32.lib", "gdi32.lib", "comdlg32.lib", "advapi32.lib"; Config = { "win32-*-*", "win64-*-*" } }},
		}
		Default "Main"
	end,
}

