/*
 * Copyright (C) 2017-2018 Tauri JayD <https://www.>
 * Copyright (c) 2016 OmniGUI Platform
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

#include "StdAfx.h"
#include "NullExtension.h"
#include "XamlCPP.h"
#include "XamlCPP/Core/Dependencies/IDependencyObject.h"


System::Reflection::Variant XamlCPP::Markup::NullExtension::ProvideValue(Core::MarkupExtensionContext * markupExtensionContext)
{
    auto PropertyMgr = Xaml::UIServiceContext->Get<XamlCPP::Core::Dependency::IDependencyPropertyMgr>();
    return PropertyMgr->GetNullValue();
}
