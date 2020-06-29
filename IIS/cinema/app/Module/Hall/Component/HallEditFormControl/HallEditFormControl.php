<?php declare(strict_types = 1);

namespace App\Module\Hall\Component\HallEditFormControl;

use App\Module\Core\Control\AbstractControl;
use App\Module\Core\Form\FormFactory;
use App\Module\Core\Latte\TRenderable;
use App\Module\Hall\Container\HallEditContainer\IHallEditContainerFactory;
use App\Module\Hall\Entity\Hall;
use App\Module\Hall\Service\HallService;
use Nette\Application\UI\Form;

/**
 * @method onSuccess(Hall $hall)
 */
class HallEditFormControl extends AbstractControl
{
    use TRenderable;

    /** @var callable[] */
    public $onSuccess = [];

    private const CONTAINER_HALL_EDIT = 'hallEdit';

    /** @var FormFactory */
    private $formFactory;

    /** @var Hall|null */
    private $hall;

    /** @var IHallEditContainerFactory */
    private $hallEditContainerFactory;

    /** @var HallService */
    private $hallService;

    public function __construct(HallService $hallService, IHallEditContainerFactory $hallEditContainerFactory, FormFactory $formFactory, ?Hall $hall = null)
    {
        $this->hall = $hall;
        $this->formFactory = $formFactory;
        $this->hallEditContainerFactory = $hallEditContainerFactory;
        $this->hallService = $hallService;
    }

    protected function createComponentForm(): Form
    {
        $form = $this->formFactory->create();
        $form->addComponent($this->hallEditContainerFactory->create($this->hall), self::CONTAINER_HALL_EDIT);
        $form->addSubmit('send', $this->hall ? 'Upravit' : 'Přidat');
        $form->onSuccess[] = [$this, 'process'];

        return $form;
    }

    public function process(Form $form): void
    {
        $hallData = $form[self::CONTAINER_HALL_EDIT]->getValues();

        if ($this->hall) {
            $this->hallService->updateExistingHall($this->hall, $hallData);
        } else {
            $this->hall = $this->hallService->addNewHall($hallData);
        }
        $this->onSuccess($this->hall);
    }
}
