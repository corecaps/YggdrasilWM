//
// Created by corecaps on 1/12/24.
//

#ifndef YGGDRASILWM_TREELAYOUTMANAGER_HPP
#define YGGDRASILWM_TREELAYOUTMANAGER_HPP
#include "LayoutManager.hpp"
class TreeLayoutManager :public LayoutManager {
	TreeLayoutManager(Display* display, Window root);
	~TreeLayoutManager() override;

	void	updateGeometry() override;
	Space	&findSpace(Client* client) override;
	Space	&findSpace(int index) override;
	void	addClient(Client* client) override;
};

#endif //YGGDRASILWM_TREELAYOUTMANAGER_HPP
