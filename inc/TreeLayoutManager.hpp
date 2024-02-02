/**
 * Yb  dP              8                w 8 Yb        dP 8b   d8
 *  YbdP  .d88 .d88 .d88 8d8b .d88 d88b w 8  Yb  db  dP  8YbmdP8
 *   YP   8  8 8  8 8  8 8P   8  8 `Yb. 8 8   YbdPYbdP   8  "  8
 *   88   `Y88 `Y88 `Y88 8    `Y88 Y88P 8 8    YP  YP    8     8
 *        wwdP wwdP
 * Yggdrasil Window Manager
 * by jgarcia <jgarcia@student.42.fr> <corecaps@gmail.com>
 * https://github.com/corecaps/YggdrasilWM
 * @file TreeLayoutManager
 * @brief handle Tree Style Layout.
 * @date 2021-06-23
 */


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

	void removeClient(Client *client);
	void removeClientRecursive(Client *client, Space* space);
	void growSpaceX(Client *client);
	void shrinkSpace(Client *client);
	Space * findSpaceRecursive(Client *client, LayoutManager::Space * space);
};

#endif //YGGDRASILWM_TREELAYOUTMANAGER_HPP
