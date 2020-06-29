<?php declare(strict_types = 1);

namespace App\Module\Core\Presenter;

use Nette\Application\Helpers;
use Nette\Application\UI\Presenter;

/**
 */
abstract class AbstractPresenter extends Presenter
{
    public function formatTemplateFiles(): array
    {
        [, $presenter] = Helpers::splitName($this->getName());
        $dir = dirname(static::getReflection()->getFileName());
        $dir = is_dir("$dir/template") ? $dir : dirname($dir);

        $files = [
            "$dir/template/$presenter/$this->view.latte",
            "$dir/template/$presenter.$this->view.latte",
        ];
        $originalFiles = parent::formatTemplateFiles();

        return \array_merge($files, $originalFiles);
    }

    public function formatLayoutTemplateFiles(): array
    {
        $layoutFiles = [];

        $reflection = $this->getReflection();

        while ('Nette\Application\UI\Presenter' !== $reflection->getName()) {
            $dir = \dirname($reflection->getFileName());
            $layoutFiles[] = "$dir/@layout.latte";
            $layoutFiles[] = "$dir/template/@layout.latte";

            $reflection = $reflection->getParentClass();
        }

        $originalLayoutFiles = parent::formatLayoutTemplateFiles();

        return \array_merge($layoutFiles, $originalLayoutFiles);
    }
}
