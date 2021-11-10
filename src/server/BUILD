# package(default_visibility = ["//visibility:public"])

cc_library(
	name = "ws_session",
	srcs = ["ws_session.cc"],
	hdrs = ["ws_session.hh"],
	deps = [
		"@boost//:boost",
	],
	linkstatic = 0,
	alwayslink = 0,

)

cc_library(
	name = "http_session",
	srcs = ["http_session.cc"],
	hdrs = ["http_session.hh"],
	deps = [
		":ws_session",
		"@boost//:boost",
	],
	alwayslink = 0,

)

cc_library(
	name = "listener",
	srcs = ["listener.cc"],
	hdrs = ["listener.hh"],
	deps = [
		":http_session",
		"@boost//:boost",
	],
	alwayslink = 0,
	visibility = ["//visibility:public"],
)

