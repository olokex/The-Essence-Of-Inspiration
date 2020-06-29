<?php declare(strict_types = 1);

namespace App\Module\User\Component\LoginFormControl;

/**
 */
interface ILoginFormControlFactory
{
    public function create(): LoginFormControl;
}
