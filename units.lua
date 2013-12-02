local path     = require "tundra.path"

DefRule {
	Name = "GenerateMoc",
	Pass = "GenerateSources",
	Command = "$(QT5_BIN)/moc $(<) -o $(@)",

	Blueprint = {
		Input = { Required = true, Type = "string" },
		Output = { Required = true, Type = "string" },
	},

	Setup = function (env, data)
		local base_name = path.get_filename_dir(data.Input) 
		return {
			InputFiles    = { data.Input },
			OutputFiles   = { "$(OBJECTDIR)/_generated/" .. base_name .. "/" .. data.Output },
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
		local base_name = path.get_filename_dir(data.Input) 
		return {
			InputFiles    = { data.Input },
			OutputFiles   = { "$(OBJECTDIR)/_generated/" .. base_name .. "/" .. data.Output },
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
		local base_name = path.get_filename_dir(data.Input) 
		return {
			InputFiles    = { data.Input },
			OutputFiles   = { "$(OBJECTDIR)/_generated/" .. base_name .. "/" .. data.Output },
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

StaticLibrary {
	Name = "Shared",
	Sources = {
		FGlob {
			Dir = "src/shared",
			Extensions = { ".c", ".cpp", ".h", ".inl" },
			Filters = {
				{ Pattern = "win32"; Config = "win32-*-*"; },
			},
		},
	},
}

local program_lib = { 
		{ 	"kernel32.lib", "user32.lib", "gdi32.lib", "comdlg32.lib", "advapi32.lib", 
			"gstreamer-0.10.lib", "glib-2.0.lib", "gobject-2.0.lib", "gstinterfaces-0.10.lib",
			"Qt5Core.lib", "Qt5Widgets.lib", "Qt5Gui.lib"; 
			Config = { "win32-*-*" } 
		},
		{ 	"gstreamer-0.10", "glib-2.0", "gobject-2.0", "gstinterfaces-0.10";
			Config = { "macosx-*-*" } 
		},
		{ 	"gstreamer-0.10", "glib-2.0", "gobject-2.0", "gstinterfaces-0.10",
			"Qt5Core", "Qt5Widgets", "Qt5Gui";
			Config = { "linux-*-*" } 
		}
	}

Program {
	Name = "MediaPlayer",
	Depends = { "Shared" },
	Sources = {
		FGlob {
			Dir = "src/mediaplayer",
			Extensions = { ".c", ".cpp", ".h", ".inl" },
			Filters = {
				{ Pattern = "win32"; Config = "win32-*-*"; },
			},
		},
		GenerateMocSources {
			Glob { 
				Dir = "src/mediaplayer/qt", 
				Extensions = { ".h" } 
			}, 
		},
		GenerateUISources {
			Glob { 
				Dir = "src/mediaplayer/qt", 
				Extensions = { ".ui" } 
			}, 
		},
		GenerateQRC {
			Input = "data/mediaplayer/application.qrc",
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
			"src",
			"$(OBJECTDIR)/_generated/src/mediaplayer/qt",
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
	Libs = program_lib,

	Frameworks = { "Cocoa", "QtCore", "QtWidgets", "QtGui", "OpenGL", "AGL"  },

}

Program {
	Name = "VoiceChat",	
	Depends = { "Shared" },
	Sources = {
		FGlob {
			Dir = "src/voicechat",
			Extensions = { ".c", ".cpp", ".h", ".inl" },
			Filters = {
				{ Pattern = "win32"; Config = "win32-*-*"; },
			},
		},
		GenerateMocSources {
			Glob { 
				Dir = "src/voicechat/qt", 
				Extensions = { ".h" } 
			}, 
		},
		GenerateUISources {
			Glob { 
				Dir = "src/voicechat/qt", 
				Extensions = { ".ui" } 
			}, 
		},
		GenerateQRC {
			Input = "data/voicechat/application.qrc",
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
			"src",
			"$(OBJECTDIR)/_generated/src/mediaplayer/qt",
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
	Libs = program_lib,

	Frameworks = { "Cocoa", "QtCore", "QtWidgets", "QtGui", "OpenGL", "AGL"  },

}

Default "VoiceChat"

