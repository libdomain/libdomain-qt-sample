# Finds Libdomain C Library
#
# Defined vars:
#     Libdomain_FOUND
#     Libdomain_INCLUDE_DIRS
# 
# Imported targets:
#     Libdomain::Libdomain

find_path(DOMAIN_INCLUDE_DIR
    NAMES libdomain/domain.h
    PATHS
        /usr/include
        /usr/local/include
        /opt/local/include
)
find_library(DOMAIN_LIBRARY domain)

mark_as_advanced(
    DOMAIN_INCLUDE_DIR
    DOMAIN_LIBRARY
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Libdomain
    FOUND_VAR Libdomain_FOUND
    REQUIRED_VARS
        DOMAIN_INCLUDE_DIR
        DOMAIN_LIBRARY
)

if(Libdomain_FOUND)
    set(Libdomain_INCLUDE_DIRS ${DOMAIN_INCLUDE_DIR})
    list(REMOVE_DUPLICATES Libdomain_INCLUDE_DIRS)
endif()

if(Libdomain_FOUND AND NOT TARGET Libdomain::Libdomain)
    add_library(Libdomain::Libdomain INTERFACE IMPORTED)
    
    target_link_libraries(Libdomain::Libdomain
        INTERFACE
            domain
    )

    target_include_directories(Libdomain::Libdomain
        INTERFACE
            ${Libdomain_INCLUDE_DIRS}
    )
endif()

