/*
 * Copyright (C) 2017-2018 Tauri JayD <https://www.>
 * Copyright (c) 2016 Austin Brunkhorst, All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "LanguageType.h"
#include "Invokable.h"

class Class;

class Constructor 
    : public LanguageType
    , public Invokable
{
    friend class Class;
public:
    Constructor(
        const Cursor &cursor, 
        const Namespace &currentNamespace, 
        Class *parent = nullptr
    );

    virtual ~Constructor(void) { } 

    void MetaInit(const Cursor &cursor, const Namespace &currentNamespace) override;
    bool ShouldCompile(void) const;
    
    Mustache::Data<std::string> CompileTemplate(
        const ReflectionParser *context
    ) const override;

    bool isConvertingConstructor = false;
    bool isCopyConstructor = false;
    bool isDefaultConstructor = false;
    bool isMoveConstructor = false;

private:
    Class *m_parent;

    bool isAccessible(void) const;

    std::string getTemplateParameters(bool isDynamic) const;
};