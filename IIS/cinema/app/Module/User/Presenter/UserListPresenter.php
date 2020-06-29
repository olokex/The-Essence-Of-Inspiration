<?php declare(strict_types = 1);

namespace App\Module\User\Presenter;

use App\Module\Front\Presenter\AbstractFrontPresenter;
use App\Module\User\Factory\UserListFactory;
use App\Module\User\Repository\UserRepository;
use App\Module\User\Service\AccessGuard;
use App\Module\User\Service\UserService;
use Ublaboo\DataGrid\DataGrid;

/**
 */
class UserListPresenter extends AbstractFrontPresenter
{
    /** @var UserListFactory @inject */
    public $userListFactory;

    /** @var UserRepository @inject */
    public $userRepository;

    /** @var UserService @inject */
    public $userService;

    public function actionDefault(): void
    {
        if (false === $this->user->isLoggedIn() || false === AccessGuard::hasAdminAccess($this->user->getEntity())) {
            $this->redirect(':Homepage:Homepage:default');
        }
    }

    protected function createComponentUserList(): DataGrid
    {
        return $this->userListFactory->create();
    }

    public function handleEdit($id): void
    {
        $userEntity = $this->userRepository->getById((int) $id);
        $this->redirect(':User:Setting:default', $userEntity);
    }

    public function handleRemove($id): void
    {
        $userEntity = $this->userRepository->getById((int) $id);
        $this->userService->removeUser($userEntity);
        $this->redirect('this');
    }
}
