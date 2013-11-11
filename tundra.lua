require "tundra.syntax.glob"
native = require "tundra.native"


local win32_config = {
	Env = {
		QT5 = native.getenv("QT5"),
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
		QT5 = native.getenv("QT5"),
		GSTREAMER_INCLUDE = "/Library/Frameworks/GStreamer.framework/Versions/0.10/Headers",
		GSTREAMER_LIBS = "/Library/Frameworks/GStreamer.framework/Versions/0.10/lib",
		CXXOPTS = {
			"-Wall", "-std=c++11", "-stdlib=libc++",
			{ "-O0", "-g"; Config = "*-*-debug" },
			{ "-O2"; Config = {"*-*-release", "*-*-production"} },
		},
		COPTS = {
			"-Wall",
			{ "-O0", "-g"; Config = "*-*-debug" },
			{ "-O2"; Config = {"*-*-release", "*-*-production"} },
		},
		LD = { "-lc++", "-F$(QT5)/lib", },
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

	Passes = {
		GenerateSources = { Name="GenerateSources", BuildOrder = 1 },
	},

	Units = "units.lua"
}

