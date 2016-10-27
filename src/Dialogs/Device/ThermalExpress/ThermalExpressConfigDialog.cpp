/*
Copyright_License {
  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2016 The XCSoar Project
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

#include "ThermalExpressConfigDialog.hpp"
#include "Device/Driver/ThermalExpress/Driver.hpp"
#include "Dialogs/WidgetDialog.hpp"
#include "Form/DataField/Enum.hpp"
#include "Language/Language.hpp"
#include "Operation/PopupOperationEnvironment.hpp"
#include "UIGlobals.hpp"
#include "Widget/RowFormWidget.hpp"


class ThermalExpressConfigurationWidget final
  : public RowFormWidget, private ActionListener {
  enum ThermalExpressWidgets {
    VOLUME,
    OK,
  };

  WidgetDialog &dialog;
  ThermalExpressDevice &device;
  int currentVolume;

public:
  ThermalExpressConfigurationWidget(const DialogLook &look, WidgetDialog &_dialog,
                             ThermalExpressDevice &_device)
    :RowFormWidget(look), dialog(_dialog), device(_device) {}

  void Prepare(ContainerWindow &parent, const PixelRect &rc) override {
    currentVolume = device.getVolume();

    AddInteger(N_("Volume"), nullptr,
           _T("%i"),
           _T("%i"),
             0, 2, 1, currentVolume);

    AddButton(_("OK"), *this, OK);
  }

  void Show(const PixelRect &rc) override {
    RowFormWidget::Show(rc);
  }

  bool Save(bool &changed) override {
    changed |= SaveValue(VOLUME, currentVolume);

    device.setVolume(currentVolume);

    return true;
  }

private:
  /* virtual methods from ActionListener */
  void OnAction(int id) override {
    bool _changed = false;

    switch (id) {
    case OK:
      dialog.GetWidget().Save(_changed);
      break;
    }
  }
};

void
dlgConfigurationThermalExpressShowModal(Device &_device)
{
  ThermalExpressDevice &device = (ThermalExpressDevice &)_device;

  const DialogLook &look = UIGlobals::GetDialogLook();

  WidgetDialog dialog(look);

  dialog.CreateAuto(UIGlobals::GetMainWindow(), _T("Thermal Express"),
                    new ThermalExpressConfigurationWidget(look, dialog, device));

  dialog.AddButton(_("Cancel"), mrCancel);
  dialog.ShowModal();
}
