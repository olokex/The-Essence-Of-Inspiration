<?php declare(strict_types = 1);

namespace App\Module\User\Presenter;

use App\Module\Front\Presenter\AbstractFrontPresenter;
use App\Module\User\Component\RegistrationFormControl\IRegistrationFormControlFactory;
use App\Module\User\Component\RegistrationFormControl\RegistrationFormControl;
use Nette\Security\User;

/**
 */
class RegistrationPresenter extends AbstractFrontPresenter
{
    /** @var User @inject */
    public $user;

    /** @var IRegistrationFormControlFactory @inject */
    public $registrationFormControlFactory;

    protected function startup(): void
    {
        parent::startup();

        if ($this->user->isLoggedIn()) {
            $this->redirect(':Homepage:Homepage:default');
        }
    }

    protected function createComponentRegistrationForm(): RegistrationFormControl
    {
        $control = $this->registrationFormControlFactory->create();
        $control->onSuccess[] = function() {
            $this->flashMessage('Registrace byla úspěšná. Nyní se můžete přihlásit.');
            $this->redirect(':Homepage:Homepage:default');
        };

        return $control;
    }
}