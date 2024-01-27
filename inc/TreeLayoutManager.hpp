//
// Created by corecaps on 1/12/24.
//

#ifndef YGGDRASILWM_TREELAYOUTMANAGER_HPP
#define YGGDRASILWM_TREELAYOUTMANAGER_HPP
#include "LayoutManager.hpp"
class TreeLayoutManager :public LayoutManager {
public:
	~TreeLayoutManager() override;
	void	updateGeometry() override;
	Space	*findSpace(Client* client) override;
	Space	*findSpace(int index) override;
	void	addClient(Client* client) override;
	TreeLayoutManager(Display* display, Window root);

	void addClientRecursive(Client *client, Space *space);

	void placeClientInSpace(Client *client, Space *space);

	void splitSpace(Client *client, Space *space, bool splitAlongX);

	void RemoveClient(Client *client);
	void RemoveClientRecursive(Client *client, Space* space);

	};

#endif //YGGDRASILWM_TREELAYOUTMANAGER_HPP
