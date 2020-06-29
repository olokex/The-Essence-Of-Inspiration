<?php declare(strict_types = 1);

namespace App\Module\Core;

use App\Module\Core\Extension\BaseExtension;
use Nette\DI\Container;
use Nette\Forms\Container as FormContainer;
use Nette\PhpGenerator\ClassType;
use Nextras\FormComponents\Controls\DateControl;
use Nextras\FormComponents\Controls\DateTimeControl;
use WebChemistry\Images\Controls\UploadControl;

/**
 */
class CoreExtension extends BaseExtension
{
    public function afterCompile(ClassType $class)
    {
        parent::afterCompile($class);
        $init = $class->getMethods()['initialize'];
        $init->addBody(__CLASS__ . '::registerControls($this);');
    }

    public static function registerControls(Container $injector): void
    {
        FormContainer::extensionMethod('addDatePicker', static function(FormContainer $container, string $name, $label = null) use ($injector) {
            $component = new DateControl($label);

            return $container[$name] = $component;
        });

        FormContainer::extensionMethod('addDateTimePicker', static function(FormContainer $container, string $name, $label = null) use ($injector) {
            $component = new DateTimeControl($label);

            return $container[$name] = $component;
        });
        FormContainer::extensionMethod('addImgUpload', static function(FormContainer $container, string $name, $label = null, ?string $namespace = null) use ($injector) {
            $component = new UploadControl($label, $namespace);

            return $container[$name] = $component;
        });
    }
}
