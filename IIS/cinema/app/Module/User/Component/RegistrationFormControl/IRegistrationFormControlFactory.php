<?php declare(strict_types = 1);

namespace App\Module\User\Component\RegistrationFormControl;

/**
 */
interface IRegistrationFormControlFactory
{
    public function create(): RegistrationFormControl;
}
