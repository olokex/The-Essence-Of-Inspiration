<?php declare(strict_types = 1);

namespace App\Module\User\Component\SettingFormControl;

use App\Module\Core\Control\AbstractControl;
use App\Module\Core\Form\FormFactory;
use App\Module\Core\Latte\TRenderable;
use App\Module\User\Container\RegistrationContainer\SettingData;
use App\Module\User\Container\SettingContainer\ISettingContainerFactory;
use App\Module\User\Entity\User;
use App\Module\User\Service\UserService;
use Nette\Application\UI\Form;

/**
 * @method onSuccess()
 */
class SettingFormControl extends AbstractControl
{
    use TRenderable;

    public const COMPONENT_SETTING = 'setting';

    /** @var callable[] */
    public $onSuccess = [];

    /** @var User */
    private $user;

    /** @var FormFactory */
    private $formFactory;

    /** @var ISettingContainerFactory */
    private $settingContainerFactory;

    /** @var UserService */
    private $userService;

    public function __construct(User $user, ISettingContainerFactory $settingContainerFactory, UserService $userService, FormFactory $formFactory)
    {
        $this->user = $user;
        $this->formFactory = $formFactory;
        $this->settingContainerFactory = $settingContainerFactory;
        $this->userService = $userService;
    }

    protected function createComponentForm(): Form
    {
        $form = $this->formFactory->create();
        $form->addComponent($this->settingContainerFactory->create($this->user), self::COMPONENT_SETTING);
        $form->addSubmit('send', 'Editovat');
        $form->onSuccess[] = [$this, 'process'];

        return $form;
    }

    public function process(Form $form): void
    {
        /** @var SettingData $settingData */
        $settingData = $form[self::COMPONENT_SETTING]->getValues('array');
        $this->userService->saveUser($this->user, $settingData);
        $this->onSuccess();
    }
}
