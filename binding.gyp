{
    "targets": [
        {
            "target_name": "labeller",
            "sources": [
                "src/nodepal.cc",
                "src/pal/layer.cpp",
                "src/pal/pal.cpp",
                "src/pal/label.cpp",
                "src/pal/feature.cpp",
                "src/pal/Geom.cpp",
                "src/pal/geomfunction.cpp",
                "src/pal/labelposition.cpp",
                "src/pal/palstat.cpp",
                "src/pal/pointset.cpp",
                "src/pal/priorityqueue.cpp",
                "src/pal/problem.cpp",
                "src/pal/util.cpp",
                "src/pal/hashtable.hpp",
                "src/pal/linkedlist.hpp",
                "src/pal/rtree.hpp"
            ],
            "cflags!": ["-fno-exceptions"],
            "cflags_cc!": ["-fno-exceptions"],
            "conditions": [
                ['OS=="mac"', {
                    "xcode_settings": {
                        "GCC_ENABLE_CPP_EXCEPTIONS": "YES"
                    }
                }]
            ],
            "libraries": ["-lgeos_c"]
        }
    ]
}