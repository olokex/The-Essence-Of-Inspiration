<?php declare(strict_types = 1);

namespace App\Module\Content\Component\ContentFormControl;

use App\Module\Content\Entity\Content;

/**
 */
interface IContentFormControlFactory
{
    public function create(?Content $content = null): ContentFormControl;
}
