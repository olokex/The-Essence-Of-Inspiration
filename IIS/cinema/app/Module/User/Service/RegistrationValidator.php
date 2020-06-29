<?php declare(strict_types = 1);

namespace App\Module\User\Service;

use App\Module\User\Component\RegistrationFormControl\RegistrationFormControl;
use App\Module\User\Container\RegistrationContainer\RegistrationContainer;
use App\Module\User\Container\RegistrationContainer\RegistrationData;
use App\Module\User\Repository\UserRepository;
use Nette\Application\UI\Form;

/**
 */
class RegistrationValidator
{
    /** @var UserRepository */
    private $userRepository;

    public function __construct(UserRepository $userRepository)
    {
        $this->userRepository = $userRepository;
    }

    public function validate(Form $form): void
    {
        /** @var RegistrationData $registrationData */
        $registrationData = $form[RegistrationFormControl::CONTAINER_REGISTRATION]->getValues();
        $this->validateEmail($form, $registrationData);
    }

    private function validateEmail(Form $form, RegistrationData $registrationData): void
    {
        if ($this->userRepository->findByEmail($registrationData->getEmail())) {
            $form[RegistrationFormControl::CONTAINER_REGISTRATION][RegistrationContainer::EMAIL]->addError('Uživatel s daným E-mailem již existuje.');
        }
    }
}
