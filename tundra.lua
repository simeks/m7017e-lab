require "tundra.syntax.glob"
native = require "tundra.native"

local win32_config = {
	Env = {
		GSTREAMER_BASEPATH = native.getenv("GSTREAMER_SDK_ROOT_X86", "."),
		GSTREAMER_INCLUDE = "$(GSTREAMER_BASEPATH)/include/",
		GSTREAMER_LIBS = "$(GSTREAMER_BASEPATH)/lib/",
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

local macosx_config = {
	Env = {
		GSTREAMER_INCLUDE = "/Library/Frameworks/GStreamer.framework/Versions/0.10/Headers",
		GSTREAMER_LIBS = "/Library/Frameworks/GStreamer.framework/Versions/0.10/lib",
		CXXOPTS = {
			"-Werror", "-Wall", "-std=c++11",
			{ "-O0", "-g"; Config = "*-*-debug" },
			{ "-O2"; Config = {"*-*-release", "*-*-production"} },
		},
		LD = { "-lc++" },
	}
}


Build {
	Configs = {
		Config {
			Name = "macosx-gcc",
			DefaultOnHost = "macosx",
			Tools = { "clang-osx" },
			Inherit = macosx_config,
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
					"$(GSTREAMER_INCLUDE)",
					"$(GSTREAMER_INCLUDE)",
					"$(GSTREAMER_INCLUDE)/atk-1.0",
					"$(GSTREAMER_INCLUDE)/cairo",
					"$(GSTREAMER_INCLUDE)/clutter-1.0",
					"$(GSTREAMER_INCLUDE)/cogl",
					"$(GSTREAMER_INCLUDE)/FLAC",
					"$(GSTREAMER_INCLUDE)/fontconfig",
					"$(GSTREAMER_INCLUDE)/freetype2",
					"$(GSTREAMER_INCLUDE)/gail-1.0",
					"$(GSTREAMER_INCLUDE)/gdk-pixbuf-2.0",
					"$(GSTREAMER_INCLUDE)/gio-win32-2.0",
					"$(GSTREAMER_INCLUDE)/glib-2.0",
					"$(GSTREAMER_INCLUDE)/gnutls",
					"$(GSTREAMER_INCLUDE)/gstreamer-0.10",
					"$(GSTREAMER_INCLUDE)/gtk-2.0",
					"$(GSTREAMER_INCLUDE)/jasper",
					"$(GSTREAMER_INCLUDE)/json-glib-1.0",
					"$(GSTREAMER_INCLUDE)/kate",
					"$(GSTREAMER_INCLUDE)/libcroco-0.6",
					"$(GSTREAMER_INCLUDE)/libdv",
					"$(GSTREAMER_INCLUDE)/libpng15",
					"$(GSTREAMER_INCLUDE)/librsvg-2.0",
					"$(GSTREAMER_INCLUDE)/libsoup-2.4",
					"$(GSTREAMER_INCLUDE)/libvisual-0.4",
					"$(GSTREAMER_INCLUDE)/libxml2",
					"$(GSTREAMER_INCLUDE)/nettle",
					"$(GSTREAMER_INCLUDE)/ogg",
					"$(GSTREAMER_INCLUDE)/opus",
					"$(GSTREAMER_INCLUDE)/orc-0.4",
					"$(GSTREAMER_INCLUDE)/pango-1.0",
					"$(GSTREAMER_INCLUDE)/pixman-1",
					"$(GSTREAMER_INCLUDE)/pycairo",
					"$(GSTREAMER_INCLUDE)/pygtk-2.0",
					"$(GSTREAMER_INCLUDE)/schroedinger-1.0",
					"$(GSTREAMER_INCLUDE)/soundtouch",
					"$(GSTREAMER_INCLUDE)/speex",
					"$(GSTREAMER_INCLUDE)/taglib",
					"$(GSTREAMER_INCLUDE)/theora",
					"$(GSTREAMER_INCLUDE)/vorbis",
					"$(GSTREAMER_INCLUDE)/vpx",
					"$(GSTREAMER_INCLUDE)/wavpack",
					
					"$(GSTREAMER_LIBS)/glib-2.0/include",
					"$(GSTREAMER_LIBS)/gtk-2.0/include",
					"$(GSTREAMER_LIBS)/libffi-3.0.11/include",
				},
				LIBPATH = {
					"$(GSTREAMER_LIBS)",
				},
				LIBS = {
				},
			},
			Libs = { 
				{ "kernel32.lib", "user32.lib", "gdi32.lib", "comdlg32.lib", "advapi32.lib", "gstreamer-0.10.lib"; Config = { "win32-*-*", "win64-*-*" } },
				{ "gstreamer-0.10"; Config = { "macosx-*-*", "macosx-*-*" } }
			},

		}
		Default "Main"
	end,
}

