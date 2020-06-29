<?php declare(strict_types = 1);

namespace App\Module\User\Presenter;

use App\Module\Core\Security\User;
use App\Module\Front\Presenter\AbstractFrontPresenter;
use App\Module\User\Component\SettingFormControl\ISettingFormControlFactory;
use App\Module\User\Component\SettingFormControl\SettingFormControl;
use App\Module\User\Entity\User as UserEntity;
use App\Module\User\Service\AccessGuard;

/**
 */
class SettingPresenter extends AbstractFrontPresenter
{
    /** @var User @inject */
    public $user;

    /** @var ISettingFormControlFactory @inject */
    public $settingFormControlFactory;

    /** @var UserEntity */
    private $userEntity;

    public function actionDefault(UserEntity $entity): void
    {
        $this->userEntity = $entity;

        if (false === $this->canAccess()) {
            $this->redirect(':Homepage:Homepage:default');
        }
    }

    private function canAccess(): bool
    {
        return $this->user->isLoggedIn() && ($this->user->getId() === $this->userEntity->getId() || AccessGuard::hasAdminAccess($this->user->getEntity()));
    }

    protected function createComponentSettingForm(): SettingFormControl
    {
        $control = $this->settingFormControlFactory->create($this->userEntity);
        $control->onSuccess[] = function () {
            $this->flashMessage('Nastavení bylo uloženo.');
            $this->redirect('this', $this->userEntity);
        };

        return $control;
    }
}
