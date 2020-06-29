<?php declare(strict_types = 1);

namespace App\Module\Homepage\Component\EventSearchFormControl;

/**
 */
interface IEventSearchFormControlFactory
{
    public function create(): EventSearchFormControl;
}
