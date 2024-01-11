/**
 * @file LayoutManager.hpp
 * @brief LayoutManager class header.
 * @date 2021-06-23
 * This class is responsible for managing the layout of the windows.
 */

#ifndef YGGDRASILWM_LAYOUTMANAGER_HPP
#define YGGDRASILWM_LAYOUTMANAGER_HPP

#include <memory>
#include <stdexcept>
#include "Client.hpp"


class LayoutManager {
public:
	struct Point {
		int	x;
		int	y;

		Point(int x, int y) : x(x), y(y) {}
	};

	class Space {
	private:
		Point					pos_;
		Point					size_;
		int						index_;
		Space*					parent_;
		std::unique_ptr<Space>	right_;
		std::unique_ptr<Space>	left_;
		Client*					client_{};

	public:
		Space(Point pos, Point size, int index, Space* parent = nullptr);
		const Point &getPos() const;
		void setPos(const Point &pos);
		const Point &getSize() const;
		void setSize(const Point &size);
		int getIndex() const;
		void setIndex(int index);
		Space *getParent() const;
		void setParent(Space *parent);
		const std::unique_ptr<Space> &getRight() const;
		void setRight(const std::unique_ptr<Space> &right);
		const std::unique_ptr<Space> &getLeft() const;
		void setLeft(const std::unique_ptr<Space> &left);
		Client *getClient() const;
		void setClient(Client *client);
	};

	class SpaceNotFoundException : public std::runtime_error {
	public:
		explicit SpaceNotFoundException(const std::string& message);
		const char * what() const noexcept override;
	};

	LayoutManager(Display* display, Window root);
	virtual ~LayoutManager() = default;

	virtual void	updateGeometry();
	virtual Space	&findSpace(Client *client);
	virtual Space	&findSpace(int index);
	virtual void	addClient(Client* client);

protected:
	Space*	rootSpace_;
	int		screen_width_;
	int		screen_height_;
	int		space_count_;
	Display	*display_;
	Window	rootWindow_;
};

#endif //YGGDRASILWM_LAYOUTMANAGER_HPP
