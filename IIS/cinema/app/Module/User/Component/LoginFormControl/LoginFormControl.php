<?php declare(strict_types = 1);

namespace App\Module\User\Component\LoginFormControl;

use App\Module\Core\Control\AbstractControl;
use App\Module\Core\Form\FormFactory;
use App\Module\Core\Latte\TRenderable;
use App\Module\User\Container\LoginContainer\ILoginContainerFactory;
use App\Module\User\Service\LoginAuthenticator;
use Nette\Application\UI\Form;

/**
 * @method onSuccess()
 */
class LoginFormControl extends AbstractControl
{
    use TRenderable;

    private const CONTAINER_LOGIN = 'login';

    /** @var callable[] */
    public $onSuccess = [];

    /** @var FormFactory */
    private $formFactory;

    /** @var ILoginContainerFactory */
    private $loginContainerFactory;

    /** @var LoginAuthenticator */
    private $loginAuthenticator;

    public function __construct(LoginAuthenticator $loginAuthenticator, ILoginContainerFactory $loginContainerFactory, FormFactory $formFactory)
    {
        $this->loginAuthenticator = $loginAuthenticator;
        $this->loginContainerFactory = $loginContainerFactory;
        $this->formFactory = $formFactory;
    }

    protected function createComponentForm(): Form
    {
        $form = $this->formFactory->create();
        $form->addComponent($this->loginContainerFactory->create(), self::CONTAINER_LOGIN);
        $form->addSubmit('send', 'Přihlásit');
        $form->onSuccess[] = [$this, 'process'];

        return $form;
    }

    public function process(Form $form): void
    {
        $loginData = $form[self::CONTAINER_LOGIN]->getValues();
        $success = $this->loginAuthenticator->authenticate($loginData);

        if ($success) {
            $this->flashMessage('Byli jste úspěšně přihlášeni.');
            $this->onSuccess();
        } else {
            $form->addError('Zadali jste nesprávné jméno nebo heslo.');
        }
    }
}
