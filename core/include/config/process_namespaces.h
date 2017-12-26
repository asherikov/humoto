/**
    @file
    @author  Alexander Sherikov

    @copyright 2017 Licensed under the Apache License, Version 2.0.
    (see @ref LICENSE or http://www.apache.org/licenses/LICENSE-2.0)

    @brief Collects defined config namespaces into a single list.
*/

#pragma once


#ifndef HUMOTO_CONFIG_NAMESPACE_1
#    define HUMOTO_CONFIG_NAMESPACE_LIST \
                 HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_0)
#else
#    ifndef HUMOTO_CONFIG_NAMESPACE_2
#        define HUMOTO_CONFIG_NAMESPACE_LIST \
                     HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_0) \
                     HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_1)
#    else
#        ifndef HUMOTO_CONFIG_NAMESPACE_3
#            define HUMOTO_CONFIG_NAMESPACE_LIST \
                         HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_0) \
                         HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_1) \
                         HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_2)
#        else
#            ifndef HUMOTO_CONFIG_NAMESPACE_4
#                define HUMOTO_CONFIG_NAMESPACE_LIST \
                             HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_0) \
                             HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_1) \
                             HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_2) \
                             HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_3)
#            else
#                ifndef HUMOTO_CONFIG_NAMESPACE_5
#                    define HUMOTO_CONFIG_NAMESPACE_LIST \
                                 HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_0) \
                                 HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_1) \
                                 HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_2) \
                                 HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_3) \
                                 HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_4)
#                else
#                    ifndef HUMOTO_CONFIG_NAMESPACE_6
#                        define HUMOTO_CONFIG_NAMESPACE_LIST \
                                     HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_0) \
                                     HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_1) \
                                     HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_2) \
                                     HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_3) \
                                     HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_4) \
                                     HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_5)
#                    else
#                        ifndef HUMOTO_CONFIG_NAMESPACE_7
#                            define HUMOTO_CONFIG_NAMESPACE_LIST \
                                         HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_0) \
                                         HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_1) \
                                         HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_2) \
                                         HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_3) \
                                         HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_4) \
                                         HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_5) \
                                         HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_6)
#                        else
#                            define HUMOTO_CONFIG_NAMESPACE_LIST \
                                         HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_0) \
                                         HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_1) \
                                         HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_2) \
                                         HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_3) \
                                         HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_4) \
                                         HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_5) \
                                         HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_6) \
                                         HUMOTO_CONFIG_NAMESPACE(HUMOTO_CONFIG_NAMESPACE_7)
#                        endif
#                    endif
#                endif
#            endif
#        endif
#    endif
#endif
