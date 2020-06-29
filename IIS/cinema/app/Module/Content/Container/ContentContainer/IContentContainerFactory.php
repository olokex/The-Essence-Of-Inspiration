<?php declare(strict_types = 1);

namespace App\Module\Content\Container\ContentContainer;

use App\Module\Content\Entity\Content;

/**
 */
interface IContentContainerFactory
{
    public function create(?Content $content = null): ContentContainer;
}
