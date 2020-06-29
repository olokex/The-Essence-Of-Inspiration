<?php declare(strict_types=1);

namespace App\Module\Homepage\Presenter;

use App\Module\Content\Repository\EventRepository;
use App\Module\Front\Presenter\AbstractFrontPresenter;
use App\Module\Homepage\Component\EventSearchFormControl\EventSearchFormControl;
use App\Module\Homepage\Component\EventSearchFormControl\IEventSearchFormControlFactory;
use App\Module\Homepage\Factory\EventListFactory;
use App\Module\Homepage\QueryBuilder\EventSearchQueryBuilder;
use Ublaboo\DataGrid\DataGrid;

/**
 */
class HomepagePresenter extends AbstractFrontPresenter
{
    /** @var EventRepository @inject */
    public $eventRepository;

    /** @var EventListFactory @inject */
    public $eventListFactory;

    /** @var IEventSearchFormControlFactory @inject */
    public $eventSearchFormControlFactory;

    /** @var EventSearchQueryBuilder @inject */
    public $eventSearchQueryBuilder;

    protected function createComponentEventSearchForm(): EventSearchFormControl
    {
        $control = $this->eventSearchFormControlFactory->create();
        $control->onSearch[] = function() {
            $this->redirect('this');
        };

        return $control;
    }

    protected function createComponentEventList(): DataGrid
    {
        $filterData = $this['eventSearchForm']->getFilterData();
        $queryBuilder = $this->eventSearchQueryBuilder->build($filterData);

        return $this->eventListFactory->create($queryBuilder);
    }

    public function handleDetail(string $id): void
    {
        $event = $this->eventRepository->getById((int) $id);
        $this->redirect(':Content:ContentDetail:default', $event->getContent());
    }
}
