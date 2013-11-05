require "tundra.syntax.glob"

Build {
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
			Libs = { { "kernel32.lib", "user32.lib", "gdi32.lib", "comdlg32.lib", "advapi32.lib"; Config = { "win32-*-*", "win64-*-*" } }},
		}
		Default "Main"
	end,
	
	Env = {
		CXXOPTS = {
			"/MD", "/WX", "/W4", "/EHsc", "/D_CRT_SECURE_NO_WARNINGS",
			{ "/Od"; Config = "*-*-debug" },
			{ "/O2"; Config = {"*-*-release", "*-*-production"} },
		},
		GENERATE_PDB = {
			{ "0"; Config = "*-vs2012-release" },
			{ "1"; Config = { "*-vs2012-debug", "*-vs2012-production" } },
		}
	},
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

}

