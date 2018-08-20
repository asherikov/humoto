/**
    @file
    @author Jan Michalczyk
    @author Alexander Sherikov
    @copyright 2014-2017 INRIA. Licensed under the Apache License, Version 2.0.
    (see @ref LICENSE or http://www.apache.org/licenses/LICENSE-2.0)

    @brief
*/

#pragma once

#define HUMOTO_CONFIGURABLE_HIERARCHY_GET_TASK_IF_ID_MATCHES(task_name, id)\
                if (#task_name == id) \
                {\
                    return (humoto::TaskSharedPointer(new task_name)); \
                }


namespace humoto
{
    /**
     * @brief Class representing the hierarchy of the problem
     */
    class HUMOTO_LOCAL ConfigurableOptimizationProblem
        :   public humoto::config::StrictConfigurableBase,
            public humoto::OptimizationProblem
    {
        #define HUMOTO_CONFIG_SECTION_ID "OptimizationProblem"
        #include HUMOTO_CONFIG_DEFINE_ACCESSORS


        public:
            /**
             * @brief Default constructor
             */
            ConfigurableOptimizationProblem()
            {
                setDefaults();
            }


            /**
             * @brief Initialize to default values
             */
            virtual void setDefaults()
            {
                task_class_names_.clear();
            }


            /**
             * @brief Read config entries
             *
             * @param[in] reader
             * @param[in] crash_on_missing_entry
             */
            template <class t_Reader>
                void readConfigEntriesTemplate( t_Reader& reader,
                                                const bool crash_on_missing_entry = true)
            {
                HUMOTO_CONFIG_READ_COMPOUND_(task_class_names)
                HUMOTO_CONFIG_READ_COMPOUND_(task_ids)


                if(task_class_names_.empty())
                {
                    HUMOTO_THROW_MSG("Enabled tasks entry empty.");
                }

                if (task_class_names_.size() != task_ids_.size())
                {
                    HUMOTO_THROW_MSG("Fields 'task_class_names' and 'task_ids' do not match.");
                }


                // reset the optimization problem
                humoto::OptimizationProblem::reset(task_class_names_.size());

                for(std::size_t i = 0;  i < task_class_names_.size(); ++i)
                {
                    if (task_class_names_[i].size() != task_ids_[i].size())
                    {
                        HUMOTO_THROW_MSG("Fields 'task_class_names' and 'task_ids' do not match.");
                    }

                    for(std::size_t j = 0; j < task_class_names_[i].size(); ++j)
                    {
                        humoto::TaskSharedPointer task = getTask(task_class_names_[i][j]);

                        if (task)
                        {
                            task->setDescription(task_ids_[i][j]);

                            // configure tasks
                            humoto::config::reader::readEntry(reader, *task, task_ids_[i][j], crash_on_missing_entry);
                            // push tasks into the stack/hierarchy
                            humoto::OptimizationProblem::pushTask(task, i);
                        }
                        else
                        {
                            HUMOTO_THROW_MSG(std::string("Unknown task name '") + task_class_names_[i][j] + "'.");
                        }
                    }
                }
                finalize();
            }


            /**
             * @brief Push task to the hierarchy
             *
             * @param[in,out] task_pointer  task
             * @param[in] level_index       level index
             * @param[in] task_class_name   name of the task class (not necessarily the same as task description!)
             */
            void pushTask(  TaskSharedPointer task_pointer,
                            const std::size_t level_index,
                            const std::string &task_class_name)
            {
                humoto::OptimizationProblem::pushTask(task_pointer, level_index);

                task_class_names_[level_index].push_back(task_class_name);
                task_ids_[level_index].push_back(task_pointer->getDescription());
            }


            /**
             * @brief Reset the optimization problem
             *
             * @param[in] number_of_levels  new number of levels
             */
            void reset( const std::size_t number_of_levels)
            {
                humoto::OptimizationProblem::reset(number_of_levels);

                task_class_names_.clear();
                task_ids_.clear();

                task_class_names_.resize(number_of_levels);
                task_ids_.resize(number_of_levels);
            }


        protected:
            std::vector<std::vector<std::string> >           task_class_names_;
            std::vector<std::vector<std::string> >           task_ids_;


        protected:
            using humoto::OptimizationProblem::pushTask;


            /**
             * @brief Fill map with all pointers to all tasks for given module
             */
            virtual humoto::TaskSharedPointer getTask(const std::string &string_id) const
            {
                HUMOTO_CONFIGURABLE_HIERARCHY_GET_TASK_IF_ID_MATCHES(TaskInfeasibleInequality, string_id);
                HUMOTO_CONFIGURABLE_HIERARCHY_GET_TASK_IF_ID_MATCHES(TaskZeroVariables, string_id);

                if (string_id == "TaskZeroSelectedVariables")
                {
                    humoto::IndexVector index_vector;
                    return (humoto::TaskSharedPointer(new humoto::TaskZeroSelectedVariables(index_vector)));
                }

                return (humoto::TaskSharedPointer());
            }



        private:
            /**
             * @brief Write config entries
             *
             * @param[in] writer
             */
            template <class t_Writer>
                void writeConfigEntriesTemplate(t_Writer& writer) const
            {
                HUMOTO_CONFIG_WRITE_COMPOUND_(task_class_names)
                HUMOTO_CONFIG_WRITE_COMPOUND_(task_ids)

                if(task_class_names_.empty())
                {
                    HUMOTO_THROW_MSG("Enabled tasks entry empty.");
                }

                for(std::size_t i = 0;  i < getNumberOfLevels(); ++i)
                {
                    for (   std::list<humoto::HierarchyLevel::TaskInfo>::const_iterator it
                                = hierarchy_[i].tasks_.begin();
                            it != hierarchy_[i].tasks_.end();
                            ++it)
                    {
                        humoto::config::writer::writeEntry(writer, *(it->ptr_), it->ptr_->getConfigSectionID());
                    }
                }
            }


            /**
             * @brief Count entries
             *
             * @return number of entries
             */
            std::size_t getNumberOfEntries() const
            {
                std::size_t num_entries = 2; // task_class_names + task_ids

                for(std::size_t i = 0;  i < getNumberOfLevels(); ++i)
                {
                    num_entries += hierarchy_[i].tasks_.size();
                }

                return (num_entries);
            }
    };
}
