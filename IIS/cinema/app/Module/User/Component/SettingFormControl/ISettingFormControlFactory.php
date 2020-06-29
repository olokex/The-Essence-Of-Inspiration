<?php declare(strict_types = 1);

namespace App\Module\User\Component\SettingFormControl;

use App\Module\User\Entity\User;

/**
 */
interface ISettingFormControlFactory
{
    public function create(User $user): SettingFormControl;
}
