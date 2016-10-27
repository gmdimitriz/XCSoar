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


#include "Device/Driver/ThermalExpress/Driver.hpp"
#include "NMEA/InputLine.hpp"
#include "NMEA/Info.hpp"
#include "Device/Port/Port.hpp"
#include "Operation/Operation.hpp"

bool
ThermalExpressDevice::ParseTXP(NMEAInputLine &line, NMEAInfo &info)
{
  // $TXP,altitude,vario
  double altitude;
  bool alt_available = line.ReadChecked(altitude);
  if (alt_available && (altitude >= -10000 && altitude <= 10000))
     info.ProvideBaroAltitudeTrue(altitude);
  
  double vario;
  bool vario_available = line.ReadChecked(vario);
  if (vario_available && (vario >= -5000 && vario <= 5000))
    info.ProvideTotalEnergyVario(vario/100.);

  return true; 
}

bool
ThermalExpressDevice::ParseCNF(NMEAInputLine &line, NMEAInfo &info)
{
  // $CNF,volume,battery,charging
  int newVolume;

  bool vol_available = line.ReadChecked(newVolume);
  if (vol_available && (newVolume >= 0 && newVolume <= 2))
    this->volume=newVolume;

  int newBattery;
  bool bat_available = line.ReadChecked(newBattery);

  if (bat_available && (newBattery >= 0 && newBattery <= 10))
  {
    info.battery_level = newBattery * 10.;
    info.battery_level_available.Update(info.clock);
  }

  return true;
}

bool
ThermalExpressDevice::ParseNMEA(const char *line, NMEAInfo &info)
{
  NMEAInputLine input_line(line);

  char buffer[5];
  input_line.Read(buffer,5);

  if (StringIsEqual(buffer,"$TXP"))
    return ParseTXP(input_line, info);
  else if (StringIsEqual(buffer,"$CNF"))
    return ParseCNF(input_line, info);
  else
    return false;
}

int
ThermalExpressDevice::getVolume()
{
  return this->volume;
}

void
ThermalExpressDevice::setVolume(int newVolume)
{
  this->volume=newVolume;
  WriteDeviceSettings();
}

void
ThermalExpressDevice::WriteDeviceSettings()
{
  NullOperationEnvironment env;
  char buffer[5];

  sprintf(buffer,"V%d\n", this->volume);
  port.FullWrite(buffer, strlen(buffer), env, 10000);
}

static Device *
ThermalExpressCreateOnPort(const DeviceConfig &config, Port &com_port)
{
  return new ThermalExpressDevice(com_port);
}

const struct DeviceRegister thermalexpress_driver = {
  _T("ThermalExpress"),
  _T("Thermal Express"),
  DeviceRegister::MANAGE,
  ThermalExpressCreateOnPort,
};
