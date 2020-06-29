<?php declare(strict_types = 1);

namespace App\Module\Homepage\Component\EventSearchFormControl;

use App\Module\Core\Control\AbstractControl;
use App\Module\Core\Form\FormFactory;
use App\Module\Core\Latte\TRenderable;
use App\Module\Homepage\Container\EventSearchContainer\EventSearchContainer;
use App\Module\Homepage\Container\EventSearchContainer\FilterData;
use App\Module\Homepage\Container\EventSearchContainer\IEventSearchContainerFactory;
use Nette\Application\UI\Form;

/**
 * @method onSearch()
 */
class EventSearchFormControl extends AbstractControl
{
    use TRenderable;

    private const COMPONENT_SEARCH = 'search';

    /** @var callable[] */
    public $onSearch = [];

    /** @var FormFactory */
    private $formFactory;

    /** @var IEventSearchContainerFactory */
    private $searchContainerFactory;

    /** @var array @persistent */
    public $filter = [];

    public function __construct(IEventSearchContainerFactory $searchContainerFactory, FormFactory $formFactory)
    {
        $this->formFactory = $formFactory;
        $this->searchContainerFactory = $searchContainerFactory;
    }

    protected function createComponentForm(): Form
    {
        $form = $this->formFactory->create();
        $form->addComponent($this->searchContainerFactory->create($this->filter), self::COMPONENT_SEARCH);
        $form->addSubmit('send', 'Hledat');
        $form->onSuccess[] = [$this, 'process'];
        
        return $form;
    }

    public function process(Form $form): void
    {
        $this->filter = $form[self::COMPONENT_SEARCH]->getValues('array');
        $this->onSearch();
    }

    public function getFilterData(): FilterData
    {
        return new FilterData(
            $this->filter[EventSearchContainer::TITLE] ?? null,
            isset($this->filter[EventSearchContainer::TYPE]) ? (int) $this->filter[EventSearchContainer::TYPE] : null,
            $this->filter[EventSearchContainer::GENRES] ?? [],
            isset($this->filter[EventSearchContainer::RATING]) ? (int) $this->filter[EventSearchContainer::RATING] : null,
        );
    }
}
