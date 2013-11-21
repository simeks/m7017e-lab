

DefRule {
	Name = "GenerateMoc",
	Pass = "GenerateSources",
	Command = "$(QT5_BIN)/moc $(<) -o $(@)",

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
	Command = "$(QT5_BIN)/rcc $(<) -o $(@) -name application",

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
	Name = "GenerateUI",
	Pass = "GenerateSources",
	Command = "$(QT5_BIN)/uic $(<) -o $(@)",

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


local function GenerateUISources(sources)
	local result = {}
	for _, src in ipairs(tundra.util.flatten(sources)) do
		result[#result + 1] = GenerateUI { Input = src, Output = "ui_" .. tundra.path.get_filename_base(src) .. ".h" }
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
				Dir = "src/qt", 
				Extensions = { ".h" } 
			}, 
		},
		GenerateUISources {
			Glob { 
				Dir = "src/qt", 
				Extensions = { ".ui" } 
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
			"$(GSTREAMER_INCLUDE)/glib-2.0",
			"$(GSTREAMER_INCLUDE)/libxml2",
			"$(GSTREAMER_LIBS)/glib-2.0/include",
			"$(QT5_INCLUDE)/QtWidgets",
			"$(QT5_INCLUDE)/QtGui",
			"$(QT5_INCLUDE)/QtCore", 
			"$(QT5_INCLUDE)/QtANGLE", 
			"$(QT5_INCLUDE)",
			"$(OBJECTDIR)/_generated/",
		},
		LIBPATH = {
			"$(GSTREAMER_LIBS)",
			"$(QT5_LIBS)",
		},
		PROGOPTS = {
			{ "/SUBSYSTEM:WINDOWS"; Config = { "win32-*-release" } },
			{ "/SUBSYSTEM:CONSOLE"; Config = { "win32-*-debug" } },
		},
	},
	Libs = { 
		{ 	"kernel32.lib", "user32.lib", "gdi32.lib", "comdlg32.lib", "advapi32.lib", 
			"gstreamer-0.10.lib", "glib-2.0.lib", "gobject-2.0.lib",
			"Qt5Core.lib", "Qt5Widgets.lib", "Qt5Gui.lib"; 
			Config = { "win32-*-*" } 
		},
		{ 	"gstreamer-0.10", "glib-2.0", "gobject-2.0";
			Config = { "macosx-*-*" } 
		},
		{ 	"gstreamer-0.10", "glib-2.0", "gobject-2.0",
			"Qt5Core", "Qt5Widgets", "Qt5Gui";
			Config = { "linux-*-*" } 
		}
	},

	Frameworks = { "Cocoa", "QtCore", "QtWidgets", "QtGui", "OpenGL", "AGL"  },

}
Default "Main"

