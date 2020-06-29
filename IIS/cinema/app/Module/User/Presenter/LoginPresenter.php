<?php declare(strict_types = 1);

namespace App\Module\User\Presenter;

use App\Module\Front\Presenter\AbstractFrontPresenter;
use App\Module\User\Component\LoginFormControl\ILoginFormControlFactory;
use App\Module\User\Component\LoginFormControl\LoginFormControl;
use App\Module\Core\Security\User;

/**
 */
class LoginPresenter extends AbstractFrontPresenter
{
    /** @var ILoginFormControlFactory @inject */
    public $loginFormControlFactory;

    /** @var User @inject */
    public $user;

    protected function startup(): void
    {
        parent::startup();

        if ($this->user->isLoggedIn()) {
            $this->redirect(':Homepage:Homepage:default');
        }
    }

    protected function createComponentLoginForm(): LoginFormControl
    {
        $control = $this->loginFormControlFactory->create();
        $control->onSuccess[] = function() {
            $this->redirect(':Homepage:Homepage:default');
        };

        return $control;
    }
}
