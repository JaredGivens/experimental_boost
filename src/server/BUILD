# package(default_visibility = ["//visibility:public"])

cc_library(
	name = "ws_session",
	srcs = ["ws_session.cc"],
	hdrs = ["ws_session.hh"],
	deps = [
		"@boost//:boost",
		"//src/game:game_session",
	],

)

cc_library(
	name = "http_session",
	srcs = ["http_session.cc"],
	hdrs = ["http_session.hh"],
	deps = [
		":ws_session",
		"@boost//:boost",
	],

)

cc_library(
	name = "listener",
	srcs = ["listener.cc"],
	hdrs = ["listener.hh"],
	deps = [
		":http_session",
		"@boost//:boost",
	],
	visibility = ["//visibility:public"],
)

