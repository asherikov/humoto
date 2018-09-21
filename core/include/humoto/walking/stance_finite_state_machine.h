/**
    @file
    @author  Alexander Sherikov
    @author  Jan Michalczyk
    @copyright 2014-2017 INRIA. Licensed under the Apache License, Version 2.0.
    (see @ref LICENSE or http://www.apache.org/licenses/LICENSE-2.0)
    @brief
*/

#pragma once

namespace humoto
{
    namespace walking
    {

        /**
         * @brief Possible stance types of the model
         *
         * @todo Consider declaration of this enum in Stance class.
         */
        class StanceType
        {
            public:
                enum Type
                {
                    /// Undefined
                    UNDEFINED = 0,

                    /// Double support
                    DS        = 1,

                    /// Left single support
                    LSS       = 2,

                    /// Right single support
                    RSS       = 3,

                    /// Transitional double support
                    TDS       = 4
                };
        };


        /**
         * @brief Possible stance subtypes of the model
         */
        class StanceSubType
        {
            public:
                enum Type
                {
                    /// Undefined
                    UNDEFINED    = 0,

                    /// First stance
                    FIRST        = 1,

                    /// Intermediate stance
                    INTERMEDIATE = 2,

                    /// Last stance
                    LAST         = 3
                };
        };



        /**
         * @brief Determines support foot for the given single support stance.
         *
         * @param[in] stance_type
         *
         * @return Left or right.
         */
        inline humoto::LeftOrRight::Type     determineSupportFoot(const StanceType::Type & stance_type)
        {
            switch(stance_type)
            {
                case StanceType::LSS:
                    return(humoto::LeftOrRight::LEFT);
                case StanceType::RSS:
                    return(humoto::LeftOrRight::RIGHT);
                default:
                    HUMOTO_THROW_MSG("Given stance type does not have corresponding support foot.");
            }
        }



        /**
         * @brief Class containing parameters of the stance finite state machine
         */
        class HUMOTO_LOCAL StanceFSMParameters : public humoto::config::ConfigurableBase
        {
            #define HUMOTO_CONFIG_SECTION_ID "StanceFSMParameters"
            #define HUMOTO_CONFIG_CONSTRUCTOR StanceFSMParameters
            #define HUMOTO_CONFIG_ENTRIES \
                    HUMOTO_CONFIG_SCALAR_(ss_duration_ms) \
                    HUMOTO_CONFIG_SCALAR_(tds_duration_ms) \
                    HUMOTO_CONFIG_SCALAR_(first_stance_duration_ms) \
                    HUMOTO_CONFIG_SCALAR_(last_stance_duration_ms) \
                    HUMOTO_CONFIG_SCALAR_(num_steps) \
                    HUMOTO_CONFIG_ENUM_(first_stance) \
                    HUMOTO_CONFIG_ENUM_(first_ss_type) \
                    HUMOTO_CONFIG_ENUM_(last_stance)
            #include HUMOTO_CONFIG_DEFINE_ACCESSORS


            public:
                StanceType::Type first_stance_;
                StanceType::Type first_ss_type_;
                StanceType::Type last_stance_;

                std::size_t ss_duration_ms_;
                std::size_t tds_duration_ms_;
                std::size_t first_stance_duration_ms_;
                std::size_t last_stance_duration_ms_;
                std::size_t num_steps_;


                /**
                 * @brief Default constructor
                 */
                StanceFSMParameters()
                {
                    setDefaults();
                }



                /**
                 * @brief Returns duration of a stance
                 *
                 * @param[in] stance_type
                 *
                 * @return
                 */
                std::size_t getDurationMs(const StanceType::Type stance_type) const
                {
                    switch(stance_type)
                    {
                        case StanceType::DS:
                            return last_stance_duration_ms_;
                        case StanceType::TDS:
                            return tds_duration_ms_;
                        case StanceType::LSS:
                            return ss_duration_ms_;
                        case StanceType::RSS:
                            return ss_duration_ms_;
                        default:
                            HUMOTO_THROW_MSG("Unsupported stance type.");
                            break;
                    }
                }


                /**
                 * @brief Default parameters of the finite state machine
                 */
                virtual void setDefaults()
                {
                    first_ss_type_ = StanceType::RSS;
                    first_stance_  = StanceType::DS;
                    last_stance_   = StanceType::DS;

                    ss_duration_ms_           = 700;
                    tds_duration_ms_          = 100;
                    first_stance_duration_ms_ = 300;
                    last_stance_duration_ms_  = 2900;

                    num_steps_ = 7;
                }
        };


        /**
         * @brief Stance
         */
        class HUMOTO_LOCAL Stance
        {
            public:
                StanceType::Type    type_;
                StanceType::Type    previous_nonds_stance_type_;
                StanceType::Type    previous_nontds_stance_type_;
                StanceSubType::Type subtype_;

                std::size_t duration_ms_;
                std::size_t total_duration_ms_;


            public:
                /**
                 * @brief Default constructor
                 */
                Stance()
                {
                    type_                        = StanceType::UNDEFINED;
                    previous_nonds_stance_type_  = StanceType::UNDEFINED;
                    previous_nontds_stance_type_ = StanceType::UNDEFINED;
                    subtype_                     = StanceSubType::UNDEFINED;
                    duration_ms_                 = 0;
                    total_duration_ms_           = 0;
                }

                /**
                 * @brief Log
                 *
                 * @param[in,out] logger logger
                 * @param[in] parent parent
                 * @param[in] name name
                 */
                void log(   humoto::Logger    &logger HUMOTO_GLOBAL_LOGGER_IF_DEFINED,
                            const LogEntryName &parent = LogEntryName(),
                            const std::string &name = "stance") const
                {
                    LogEntryName subname = parent; subname.add(name);

                    logger.log(LogEntryName(subname).add("type")                       , type_                       );
                    logger.log(LogEntryName(subname).add("previous_nonds_stance_type") , previous_nonds_stance_type_ );
                    logger.log(LogEntryName(subname).add("previous_nontds_stance_type"), previous_nontds_stance_type_);
                    logger.log(LogEntryName(subname).add("subtype")                    , subtype_                    );
                    logger.log(LogEntryName(subname).add("duration_ms")                , duration_ms_                );
                    logger.log(LogEntryName(subname).add("total_duration_ms")          , total_duration_ms_          );
                }
        };


        /**
         * @brief Finite state machine for walking. [initialize_contact_walk_fsm.m]
         *
         *        Supports online states sequence generation
         */
        class HUMOTO_LOCAL StanceFiniteStateMachine
        {
            public:
                bool                is_tds_started_;
                std::size_t         ss_states_to_termination_;
                std::size_t         current_time_ms_;
                Stance              current_stance_;
                StanceFSMParameters sfsm_params_;


            public:
                StanceFiniteStateMachine()
                {
                    finalize();
                }

                void setParameters(const StanceFSMParameters &sfsm_params)
                {
                    sfsm_params_ = sfsm_params;
                    finalize();
                }


                virtual void finalize()
                {
                    HUMOTO_ASSERT(sfsm_params_.num_steps_ != 1, "Unsupported number of steps.");

                    ss_states_to_termination_                    = sfsm_params_.num_steps_;
                    current_time_ms_                             = 0;
                    is_tds_started_                              = false;
                    current_stance_.previous_nontds_stance_type_ = StanceType::UNDEFINED;
                    current_stance_.subtype_                     = StanceSubType::FIRST;
                    current_stance_.type_                        = sfsm_params_.first_stance_;
                    current_stance_.duration_ms_                 = sfsm_params_.first_stance_duration_ms_;
                    current_stance_.total_duration_ms_           = sfsm_params_.getDurationMs(sfsm_params_.first_stance_);

                    switch (current_stance_.type_)
                    {
                        case StanceType::LSS:
                        case StanceType::RSS:
                            current_stance_.previous_nonds_stance_type_ = current_stance_.type_;
                            break;
                        case StanceType::DS:
                            switch(sfsm_params_.first_ss_type_)
                            {
                                case StanceType::RSS:
                                    current_stance_.previous_nonds_stance_type_ = StanceType::LSS;
                                    break;
                                case StanceType::LSS:
                                    current_stance_.previous_nonds_stance_type_ = StanceType::RSS;
                                    break;
                                case StanceType::UNDEFINED:
                                    current_stance_.previous_nonds_stance_type_ = StanceType::UNDEFINED;
                                    sfsm_params_.num_steps_ = 0;
                                    break;
                                default:
                                    HUMOTO_THROW_MSG("Misformed walking options.");
                                    break;
                            }
                            break;
                        default:
                            HUMOTO_THROW_MSG("Misformed walking options.");
                            break;
                    }
                }


                /**
                 * @brief Preview sequence of Stances of the FSM
                 *
                 * @param[in] preview_duration_ms
                 *
                 * @return
                 */
                template <class t_Stance>
                    std::vector<t_Stance> previewStances(const std::size_t preview_duration_ms) const
                {
                    bool not_enough_states = false;

                    std::vector<t_Stance> stances;
                    t_Stance    stance;
                    Stance *stance_ptr = static_cast<Stance*>(&stance);

                    StanceFiniteStateMachine stance_fsm = StanceFiniteStateMachine(*this);


                    for (std::size_t duration_ms = preview_duration_ms; duration_ms > 0; )
                    {
                        stance_ptr->type_        = stance_fsm.current_stance_.type_;
                        stance_ptr->subtype_     = stance_fsm.current_stance_.subtype_;
                        stance_ptr->duration_ms_ = std::min(stance_fsm.current_stance_.duration_ms_ - stance_fsm.current_time_ms_, duration_ms);
                        stance_ptr->total_duration_ms_           = sfsm_params_.getDurationMs(stance_ptr->type_);
                        stance_ptr->previous_nonds_stance_type_  = stance_fsm.current_stance_.previous_nonds_stance_type_;
                        stance_ptr->previous_nontds_stance_type_ = stance_fsm.current_stance_.previous_nontds_stance_type_;

                        if(stance_fsm.current_time_ms_ + duration_ms <= stance_fsm.current_stance_.duration_ms_)
                        {
                            duration_ms = 0;
                            stance_fsm.current_time_ms_ = stance_fsm.current_time_ms_ + duration_ms;
                        }
                        else
                        {
                            duration_ms -= (stance_fsm.current_stance_.duration_ms_ - stance_fsm.current_time_ms_);

                            if((stance_fsm.ss_states_to_termination_ == 0) && (stance_fsm.current_stance_.type_ == StanceType::DS))
                            {
                                not_enough_states = true;
                                break;
                            }
                            else
                            {
                                stance_fsm.shiftStance();
                            }
                        }

                        stances.push_back(stance);
                    }

                    //always add one state afted TDS
                    if((not_enough_states == false) && (stances.back().type_ == StanceType::TDS))
                    {
                        if(stance_fsm.current_stance_.type_ == StanceType::TDS)
                        {
                            stance_fsm.shiftStance();
                        }

                        stance_ptr->type_                        = stance_fsm.current_stance_.type_;
                        stance_ptr->subtype_                     = stance_fsm.current_stance_.subtype_;
                        stance_ptr->duration_ms_                 = 0;
                        stance_ptr->total_duration_ms_           = 0;
                        stance_ptr->previous_nonds_stance_type_  = stance_fsm.current_stance_.previous_nonds_stance_type_;
                        stance_ptr->previous_nontds_stance_type_ = stance_fsm.current_stance_.previous_nontds_stance_type_;

                        stances.push_back(stance);
                    }

                    return (stances);
                }


                /**
                 * @brief Preview sequence of Stances of the FSM
                 *
                 * @param[in] N
                 *
                 * @return
                 */
                template <class t_Stance>
                    std::vector<t_Stance> previewNStances(const std::size_t N) const
                {
                    bool not_enough_states = false;

                    std::vector<t_Stance> stances;
                    t_Stance    stance;
                    Stance *stance_ptr = static_cast<Stance*>(&stance);

                    StanceFiniteStateMachine stance_fsm = StanceFiniteStateMachine(*this);


                    for (std::size_t i = 0; i < N; ++i)
                    {
                        *stance_ptr = stance_fsm.current_stance_;
                        if(0 == i)
                        {
                            stance_ptr->duration_ms_ = stance_fsm.current_stance_.duration_ms_ - stance_fsm.current_time_ms_;
                        }

                        if((stance_fsm.ss_states_to_termination_ == 0) && (stance_fsm.current_stance_.type_ == StanceType::DS))
                        {
                            not_enough_states = true;
                            break;
                        }

                        stance_fsm.shiftStance();

                        stances.push_back(stance);
                    }


                    //always add one state afted TDS
                    if((not_enough_states == false) && (stances.back().type_ == StanceType::TDS))
                    {
                        if(stance_fsm.current_stance_.type_ == StanceType::TDS)
                        {
                            stance_fsm.shiftStance();
                        }
                        *stance_ptr = stance_fsm.current_stance_;

                        stance_ptr->duration_ms_                 = 0;
                        stance_ptr->total_duration_ms_           = 0;

                        stances.push_back(stance);
                    }

                    return (stances);
                }


                /**
                 * @brief Preview next walk state of the FSM
                 */
                Stance getNextStance() const
                {
                    StanceFiniteStateMachine stance_fsm = StanceFiniteStateMachine(*this);
                    stance_fsm.shiftStance();
                    return stance_fsm.current_stance_;
                }


                /**
                 * @brief Constructor
                 *
                 * @param[in] sfsm_params
                 */
                StanceFiniteStateMachine(const StanceFSMParameters &sfsm_params)
                {
                    sfsm_params_ = sfsm_params;
                    finalize();
                }


                /**
                 * @brief Shift time
                 *
                 * @param[in] shift_time_ms
                 */
                void shiftTime(const std::size_t shift_time_ms)
                {
                    for (std::size_t time_ms = shift_time_ms; time_ms > 0; )
                    {
                        is_tds_started_ = false;

                        if(current_time_ms_ + time_ms < current_stance_.duration_ms_)
                        {
                            current_time_ms_ = current_time_ms_ + time_ms;
                            time_ms = 0;
                        }
                        else
                        {
                            time_ms -= (current_stance_.duration_ms_ - current_time_ms_);
                            shiftStance();
                        }
                    }
                }


                /**
                 * @brief Shift state of the model
                 */
                void shiftStance()
                {
                    is_tds_started_ = true;

                    current_stance_.subtype_ = StanceSubType::INTERMEDIATE;

                    switch(current_stance_.type_)
                    {
                        case StanceType::LSS:
                            current_stance_.previous_nonds_stance_type_  = current_stance_.type_;
                            current_stance_.previous_nontds_stance_type_ = current_stance_.type_;
                            current_stance_.type_        = StanceType::TDS;
                            current_time_ms_ = 0;
                            break;

                        case StanceType::RSS:
                            current_stance_.previous_nonds_stance_type_  = current_stance_.type_;
                            current_stance_.previous_nontds_stance_type_ = current_stance_.type_;
                            current_stance_.type_        = StanceType::TDS;
                            current_time_ms_ = 0;
                            break;

                        case StanceType::TDS:
                            is_tds_started_ = false;

                            if(ss_states_to_termination_ == 0)
                            {
                                current_stance_.subtype_     = StanceSubType::LAST;
                                current_stance_.type_        = StanceType::DS;
                                current_time_ms_ = 0;
                            }
                            else
                            {
                                if(ss_states_to_termination_ > 0)
                                {
                                    ss_states_to_termination_ = ss_states_to_termination_ - 1;
                                }

                                if(current_stance_.previous_nonds_stance_type_ == StanceType::LSS)
                                {
                                    current_stance_.type_        = StanceType::RSS;
                                    current_time_ms_ = 0;
                                }
                                else if(current_stance_.previous_nonds_stance_type_ == StanceType::RSS)
                                {
                                    current_stance_.type_        = StanceType::LSS;
                                    current_time_ms_ = 0;
                                }
                                else
                                {
                                    HUMOTO_THROW_MSG("FSM is in an incorrect state.");
                                }
                            }
                            break;

                        case StanceType::DS:
                            current_stance_.previous_nontds_stance_type_ = current_stance_.type_;

                            if(ss_states_to_termination_ == 0)
                            {
                                current_stance_.type_        = StanceType::DS;
                                current_time_ms_ = 0;
                            }
                            else
                            {
                                current_stance_.type_        = StanceType::TDS;
                                current_time_ms_ = 0;
                            }
                            break;

                        default:
                            HUMOTO_THROW_MSG("FSM is in an incorrect state.");
                    }

                    current_stance_.duration_ms_ = sfsm_params_.getDurationMs(current_stance_.type_);
                    current_stance_.total_duration_ms_ = sfsm_params_.getDurationMs(current_stance_.type_);
                }


                bool isStanceStarted() const
                {
                    return (0 == current_time_ms_);
                }


                /**
                 * @brief Log
                 *
                 * @param[in,out] logger logger
                 * @param[in] parent parent
                 * @param[in] name name
                 */
                void log(   humoto::Logger & logger HUMOTO_GLOBAL_LOGGER_IF_DEFINED,
                            const LogEntryName &parent = LogEntryName(),
                            const std::string &name = "stance_fsm") const
                {
                    LogEntryName subname = parent; subname.add(name);

                    current_stance_.log(logger, subname, "current_stance");
                    logger.log(LogEntryName(subname).add("ss_states_to_termination"), ss_states_to_termination_);
                    logger.log(LogEntryName(subname).add("current_time")            , current_time_ms_         );
                    logger.log(LogEntryName(subname).add("is_tds_started")          , is_tds_started_          );
                }
        };
    }//end namespace walking
}//end namespace humoto

