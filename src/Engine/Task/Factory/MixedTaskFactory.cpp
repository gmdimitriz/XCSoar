/* Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2012 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
 */

#include "MixedTaskFactory.hpp"
#include "TaskFactoryConstraints.hpp"
#include "Util/Macros.hpp"

static gcc_constexpr_data TaskFactoryConstraints mixed_constraints = {
  true,
  false,
  false,
  false,
  true,
  2, 10,
};

static gcc_constexpr_data TaskPointFactoryType mixed_start_types[] = {
  TaskPointFactoryType::START_LINE,
  TaskPointFactoryType::START_CYLINDER,
  TaskPointFactoryType::START_BGA,
  TaskPointFactoryType::START_SECTOR,
};

static gcc_constexpr_data TaskPointFactoryType mixed_im_types[] = {
  TaskPointFactoryType::FAI_SECTOR,
  TaskPointFactoryType::AST_CYLINDER,
  TaskPointFactoryType::AAT_CYLINDER,
  TaskPointFactoryType::AAT_SEGMENT,
  TaskPointFactoryType::AAT_ANNULAR_SECTOR,
  TaskPointFactoryType::KEYHOLE_SECTOR,
  TaskPointFactoryType::BGAFIXEDCOURSE_SECTOR,
  TaskPointFactoryType::BGAENHANCEDOPTION_SECTOR,
};

static gcc_constexpr_data TaskPointFactoryType mixed_finish_types[] = {
  TaskPointFactoryType::FINISH_SECTOR,
  TaskPointFactoryType::FINISH_LINE,
  TaskPointFactoryType::FINISH_CYLINDER,
};

MixedTaskFactory::MixedTaskFactory(OrderedTask& _task,
                                   const TaskBehaviour &tb)
  :AbstractTaskFactory(mixed_constraints, _task, tb,
                       LegalPointConstArray(mixed_start_types,
                                            ARRAY_SIZE(mixed_start_types)),
                       LegalPointConstArray(mixed_im_types,
                                            ARRAY_SIZE(mixed_im_types)),
                       LegalPointConstArray(mixed_finish_types,
                                            ARRAY_SIZE(mixed_finish_types)))
{
}
