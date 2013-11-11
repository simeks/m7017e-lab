

DefRule {
	Name = "GenerateMoc",
	Pass = "GenerateSources",
	Command = "$(QT5)/bin/moc $(<) -o $(@)",

	Blueprint = {
		Input = { Required = true, Type = "string" },
		Output = { Required = true, Type = "string" },
	},

	Setup = function (env, data)
		return {
			InputFiles    = { data.Input },
			OutputFiles   = { "$(OBJECTDIR)/_generated/" .. data.Output },
		}
	end,
}

DefRule {
	Name = "GenerateQRC",
	Pass = "GenerateSources",
	Command = "$(QT5)/bin/rcc $(<) -o $(@) -name application",

	Blueprint = {
		Input = { Required = true, Type = "string" },
		Output = { Required = true, Type = "string" },
	},

	Setup = function (env, data)
		return {
			InputFiles    = { data.Input },
			OutputFiles   = { "$(OBJECTDIR)/_generated/" .. data.Output },
		}
	end,
}




local function GenerateMocSources(sources)
	local result = {}
	for _, src in ipairs(tundra.util.flatten(sources)) do
		result[#result + 1] = GenerateMoc { Input = src, Output = tundra.path.get_filename_base(src) .. "_moc.cpp" }
	end
	return result
end


Program {
	Name = "Main",
	Sources = {
		FGlob {
			Dir = "src",
			Extensions = { ".c", ".cpp", ".h", ".inl" },
			Filters = {
				{ Pattern = "win32"; Config = "win32-*-*"; },
			},
		},
		GenerateMocSources {
			Glob { 
				Dir = "src", 
				Extensions = { ".h" } 
			}, 
		},
		GenerateQRC {
			Input = "data/application.qrc",
			Output = "application.cpp"
		}
	},
	Env = {
		CPPPATH = { 
			"$(GSTREAMER_INCLUDE)",
			"$(GSTREAMER_INCLUDE)/gstreamer-0.10",
			"$(QT5)/include/QtWidgets",
			"$(QT5)/include/QtGui",
			"$(QT5)/include/QtCore", 
			"$(QT5)/include/QtANGLE", 
			"$(QT5)/include",	
		},
		LIBPATH = {
			"$(GSTREAMER_LIBS)",
			"$(QT5)/lib",
		},
		PROGOPTS = {
			{ "/SUBSYSTEM:WINDOWS"; Config = { "win32-*-*" } },
		},
	},
	Libs = { 
		{ 	"kernel32.lib", "user32.lib", "gdi32.lib", "comdlg32.lib", "advapi32.lib", 
			"gstreamer-0.10.lib",
			"Qt5Core.lib", "Qt5Widgets.lib", "Qt5Gui.lib"; 
			Config = { "win32-*-*" } 
		},
		{ 	"gstreamer-0.10"; 
			Config = { "macosx-*-*", "macosx-*-*" } 
		}
	},

	Frameworks = { "Cocoa", "QtCore", "QtWidgets", "QtGui", "OpenGL", "AGL"  },

}
Default "Main"
