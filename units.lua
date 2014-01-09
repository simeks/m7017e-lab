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
	Env = {	
		CPPPATH = { 
			"$(GSTREAMER_INCLUDE)",
			"$(GSTREAMER_INCLUDE)/gstreamer-0.10",
			"$(GSTREAMER_INCLUDE)/glib-2.0",
			"$(GSTREAMER_INCLUDE)/libxml2",
			"$(GSTREAMER_LIBS)/glib-2.0/include",
			"src",
		},
	},
}

local program_lib = { 
		{ 	"kernel32.lib", "user32.lib", "gdi32.lib", "comdlg32.lib", "advapi32.lib", 
			"gstreamer-0.10.lib", "glib-2.0.lib", "gobject-2.0.lib", "gstinterfaces-0.10.lib",
			"Qt5Core.lib", "Qt5Widgets.lib", "Qt5Gui.lib", "Qt5Network.lib", "WS2_32.lib"; 
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
				Dir = "src/voicechat", 
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
			"$(QT5_INCLUDE)/QtNetwork", 
			"$(QT5_INCLUDE)",
			"src",
			"$(OBJECTDIR)/_generated/src/voicechat/qt",
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

	Frameworks = { "Cocoa", "QtCore", "QtWidgets", "QtGui", "OpenGL", "AGL", "QtNetwork"  },

}

Program {
	Name = "VoiceChatServer",	
	Depends = { "Shared" },
	Sources = {
		FGlob {
			Dir = "src/voicechat_server",
			Extensions = { ".c", ".cpp", ".h", ".inl" },
			Filters = {
				{ Pattern = "win32"; Config = "win32-*-*"; },
			},
		},
		GenerateMocSources {
			Glob { 
				Dir = "src/voicechat_server", 
				Extensions = { ".h" } 
			}, 
		},
	},
	Env = {	
		CPPPATH = { 
			"src",
			"$(GSTREAMER_INCLUDE)",
			"$(GSTREAMER_INCLUDE)/gstreamer-0.10",
			"$(GSTREAMER_INCLUDE)/glib-2.0",
			"$(GSTREAMER_INCLUDE)/libxml2",
			"$(GSTREAMER_LIBS)/glib-2.0/include",
			"$(QT5_INCLUDE)/QtCore", 
			"$(QT5_INCLUDE)/QtNetwork", 
			"$(QT5_INCLUDE)",
		},
		LIBPATH = {
			"$(GSTREAMER_LIBS)",
			"$(QT5_LIBS)",
		},
		PROGOPTS = {
			{ "/SUBSYSTEM:CONSOLE"; Config = { "win32-*-*" } },
		},
	},
	Libs = program_lib,

	Frameworks = { "QtCore", "QtNetwork"  },	
}

Program {
	Name = "IPTelephone",
	Depends = { "Shared" },
	Sources = {
		FGlob {
			Dir = "src/ip_telephone",
			Extensions = { ".c", ".cpp", ".h", ".inl" },
			Filters = {
				{ Pattern = "win32"; Config = "win32-*-*"; },
			},
		},
		GenerateMocSources {
			Glob { 
				Dir = "src/ip_telephone", 
				Extensions = { ".h" } 
			}, 
		},
		GenerateUISources {
			Glob { 
				Dir = "src/ip_telephone/qt", 
				Extensions = { ".ui" } 
			}, 
		},
		GenerateQRC {
			Input = "data/ip_telephone/application.qrc",
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
			"$(QT5_INCLUDE)/QtMultimedia",			
			"$(QT5_INCLUDE)",
			"src",
			"$(OBJECTDIR)/_generated/src/ip_telephone/qt",
			"dependencies/pjproject-2.1.0/pjlib/include",
			"dependencies/pjproject-2.1.0/pjlib-util/include",
			"dependencies/pjproject-2.1.0/pjmedia/include",
			"dependencies/pjproject-2.1.0/pjnath/include",
			"dependencies/pjproject-2.1.0/pjsip/include",
		},
		LIBPATH = {
			"$(GSTREAMER_LIBS)",
			"$(QT5_LIBS)",
			"dependencies/pjproject-2.1.0/pjsip/lib",
			"dependencies/pjproject-2.1.0/pjlib-util/lib",
			"dependencies/pjproject-2.1.0/lib",
		},
		PROGOPTS = {
			{ "/SUBSYSTEM:WINDOWS"; Config = { "win32-*-release" } },
			{ "/SUBSYSTEM:CONSOLE"; Config = { "win32-*-debug" } },
		},
	},
	Libs = { 
		{ 	"kernel32.lib", "user32.lib", "gdi32.lib", "comdlg32.lib", "advapi32.lib", "Ole32.lib",
			"gstreamer-0.10.lib", "glib-2.0.lib", "gobject-2.0.lib", "gstinterfaces-0.10.lib",
			"Qt5Core.lib", "Qt5Widgets.lib", "Qt5Gui.lib", "Qt5Network.lib", "Qt5Multimedia.lib", "WS2_32.lib", 
			"pjsip-core-i386-Win32-vc8-Release-Static.lib", 
			"pjsip-simple-i386-Win32-vc8-Release-Static.lib", 
			"pjsua-lib-i386-Win32-vc8-Release-Static.lib", 
			"pjsip-ua-i386-Win32-vc8-Release-Static.lib", 
			"pjlib-util-i386-Win32-vc8-Release-Static.lib", 
			"libpjproject-i386-Win32-vc8-Release-Static.lib"; 
			Config = { "win32-*-*" } 
		},
		{ 	"gstreamer-0.10", "glib-2.0", "gobject-2.0", "gstinterfaces-0.10";
			Config = { "macosx-*-*" } 
		},
		{ 	"gstreamer-0.10", "glib-2.0", "gobject-2.0", "gstinterfaces-0.10",
			"Qt5Core", "Qt5Widgets", "Qt5Gui";
			Config = { "linux-*-*" } 
		}
	},

	Frameworks = { "Cocoa", "QtCore", "QtWidgets", "QtGui", "OpenGL", "AGL"  },

}

Default "IPTelephone"

