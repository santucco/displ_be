/*
 *  Config
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * Copyright (C) 2016 EPAM Systems Inc.
 */

#ifndef SRC_CONFIG_HPP_
#define SRC_CONFIG_HPP_

#include <exception>
#include <memory>
#include <string>

#include <libconfig.h++>

#include <xen/be/Log.hpp>

/***************************************************************************//**
 * @defgroup config Config
 * Config related classes.
 ******************************************************************************/

/***************************************************************************//**
 * Exception generated by Config class.
 * @ingroup config
 ******************************************************************************/
class ConfigException : public std::exception
{
public:

	explicit ConfigException(const std::string& msg) : mMsg(msg) {};
	virtual ~ConfigException() {}

	/**
	 * returns error message
	 */
	const char* what() const throw() { return mMsg.c_str(); };

private:
	std::string mMsg;
};

/***************************************************************************//**
 * Provides different settings for backends.
 * @ingroup config
 ******************************************************************************/
class Config
{
public:

	/**
	 * Display modes
	 */
	enum class DisplayMode
	{
		DRM,   //!< DRM
		WAYLAND//!< Wayland
	};

	/**
	 * @param fileName
	 */
	Config(const std::string& fileName);

	/**
	 * Returns display mode
	 */
	DisplayMode displayMode() const { return mDisplayMode; }

	/**
	 * Returns number of defined virtual wayland connectors
	 */
	int wlConnectorsCount() const { return mWlConnectorsCount; }

	/**
	 * Returns parameters of virtual wayland connector
	 * @param[in]  idx    index of connector
	 * @param[out] name   connector name
	 */
	void wlConnector(int idx, std::string& name);

	/**
	 * Returns number of defined keyboards
	 */
	int inputKeyboardsCount() const { return mInputKeyboardsCount; }

	/**
	 * Returns parameters of defined keyboard
	 * @param[in]  idx     keyboard index
	 * @param[out] id      keyboard id
	 * @param[out] wayland indicates if it is virtual wayland keyboard
	 * @param[out] name    if wayland is <i>true</i>, name of associated
	 * connector. Otherwise, device name /dev/input/event...
	 */
	void inputKeyboard(int idx, int& id, bool& wayland, std::string& name);

	/**
	 * Return number of defined pointers
	 */
	int inputPointersCount() const { return mInputPointersCount; }

	/**
	 * Returns parameters of defined pointer
	 * @param[in]  idx     pointer index
	 * @param[out] id      pointer id
	 * @param[out] wayland indicates if it is virtual wayland pointer
	 * @param[out] name    if wayland is <i>true</i>, name of associated
	 * connector. Otherwise, device name /dev/input/event...
	 */
	void inputPointer(int idx, int& id, bool& wayland, std::string& name);

	/**
	 * Return number of defined touches
	 */
	int inputTouchesCount() const { return mInputTouchesCount; }

	/**
	 * Returns parameters of defined touch
	 * @param[in]  idx     touch index
	 * @param[out] id      touch id
	 * @param[out] wayland indicates if it is virtual wayland touch
	 * @param[out] name    if wayland is <i>true</i>, name of associated
	 * connector. Otherwise, device name /dev/input/event...
	 */
	void inputTouch(int idx, int& id, bool& wayland, std::string& name);

	/**
	 * Returns connector name for specified frontend
	 * @param[in] domName domain name
	 * @param[in] devId   device id
	 * @param[in] idx     connector index
	 * @return connector name
	 */
	std::string domConnectorName(const std::string& domName, uint16_t devId,
								 int idx);

	/**
	 * Return keyboard id for specified frontend
	 * @param[in]  domName domain name
	 * @param[in]  devId   device id
	 * @param[out] id      keyboard id
	 * @return <i>true</i> if keyboard id is defined
	 */
	bool domKeyboardId(const std::string& domName, uint16_t devId, int& id);

	/**
	 * Return pointer id for specified frontend
	 * @param[in]  domName domain name
	 * @param[in]  devId   device id
	 * @param[out] id      pointer id
	 * @return <i>true</i> if pointer id is defined
	 */
	bool domPointerId(const std::string& domName, uint16_t devId, int& id);

	/**
	 * Return touch id for specified frontend
	 * @param[in]  domName domain name
	 * @param[in]  devId   device id
	 * @param[out] id      touch id
	 * @return <i>true</i> if touch id is defined
	 */
	bool domTouchId(const std::string& domName, uint16_t devId, int& id);

private:

	const char* cDefaultCfgName = "displ_be.cfg";

	XenBackend::Log mLog;
	libconfig::Config mConfig;

	DisplayMode mDisplayMode;
	int mWlConnectorsCount;
	int mInputKeyboardsCount;
	int mInputPointersCount;
	int mInputTouchesCount;

	void initCachedValues();
	DisplayMode readDisplayMode();
	int readWlConnectorsCount();
	int readInputKeyboardsCount();
	int readInputPointersCount();
	int readInputTouchesCount();
	void readInputSection(const std::string& sectionName, int idx,
						  int& id, bool& wayland, std::string& name);
	int readInputsCount(const std::string& sectionName);
	bool readInputId(const std::string& settingName, const std::string& domName,
					 uint16_t devId, int& id);
	libconfig::Setting& findSettingByDomain(const std::string& sectionName,
											const std::string& domName,
											uint16_t devId);
};

typedef std::shared_ptr<Config> ConfigPtr;

#endif /* SRC_CONFIG_HPP_ */
