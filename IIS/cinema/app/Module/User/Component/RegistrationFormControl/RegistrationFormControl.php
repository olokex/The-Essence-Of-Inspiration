<?php declare(strict_types = 1);

namespace App\Module\User\Component\RegistrationFormControl;

use App\Module\Core\Control\AbstractControl;
use App\Module\Core\Form\FormFactory;
use App\Module\Core\Latte\TRenderable;
use App\Module\User\Container\RegistrationContainer\IRegistrationContainerFactory;
use App\Module\User\Service\RegistrationService;
use App\Module\User\Service\RegistrationValidator;
use Nette\Application\UI\Form;

/**
 * @method onSuccess()
 */
class RegistrationFormControl extends AbstractControl
{
    use TRenderable;

    public const CONTAINER_REGISTRATION = 'registration';

    /** @var callable[] */
    public $onSuccess = [];

    /** @var FormFactory */
    private $formFactory;

    /** @var IRegistrationContainerFactory */
    private $registrationContainerFactory;

    /** @var RegistrationValidator */
    private $registrationValidator;

    /** @var RegistrationService */
    private $registrationService;

    public function __construct(
        RegistrationValidator $registrationValidator,
        RegistrationService $registrationService,
        IRegistrationContainerFactory $registrationContainerFactory,
        FormFactory $formFactory
    )
    {
        $this->registrationValidator = $registrationValidator;
        $this->formFactory = $formFactory;
        $this->registrationContainerFactory = $registrationContainerFactory;
        $this->registrationService = $registrationService;
    }

    protected function createComponentForm(): Form
    {
        $form = $this->formFactory->create();
        $form->addComponent($this->registrationContainerFactory->create(), self::CONTAINER_REGISTRATION);
        $form->addSubmit('send', 'Registrovat');
        $form->onValidate[] = [$this, 'validate'];
        $form->onSuccess[] = [$this, 'process'];

        return $form;
    }

    public function validate(Form $form): void
    {
        $this->registrationValidator->validate($form);
    }

    public function process(Form $form): void
    {
        $registrationData = $form[self::CONTAINER_REGISTRATION]->getValues();
        $this->registrationService->register($registrationData);
        $this->onSuccess();
    }
}
