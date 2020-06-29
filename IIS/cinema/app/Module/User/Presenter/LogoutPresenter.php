<?php declare(strict_types = 1);

namespace App\Module\User\Presenter;

use App\Module\Front\Presenter\AbstractFrontPresenter;
use Nette\Security\User;

/**
 */
class LogoutPresenter extends AbstractFrontPresenter
{
    /** @var User @inject */
    public $user;

    public function startup(): void
    {
        parent::startup();

        if ($this->user->isLoggedIn()) {
            $this->user->logout();
            $this->flashMessage('Byli jste odhlášeni.');
        }
        $this->redirect(':Homepage:Homepage:default');
    }
}
