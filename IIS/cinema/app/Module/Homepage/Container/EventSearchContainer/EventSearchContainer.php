<?php declare(strict_types = 1);

namespace App\Module\Homepage\Container\EventSearchContainer;

use App\Module\Content\Repository\GenreRepository;
use App\Module\Content\Repository\TypeRepository;
use Nette\Forms\Container;

/**
 */
class EventSearchContainer extends Container
{
    public const
        TITLE = 'title',
        TYPE = 'type',
        GENRES = 'genres',
        RATING = 'rating';

    public function __construct(array $defaults, TypeRepository $typeRepository, GenreRepository $genreRepository)
    {
        $this->addText(self::TITLE, 'Název')
            ->setNullable()
            ->setHtmlAttribute('placeholder', 'Název');
        $this->addSelect(self::TYPE, 'Typ', $typeRepository->getPairs())
            ->setPrompt('-- Vyberte --');
        $this->addMultiSelect(self::GENRES, 'Žánry', $genreRepository->getPairs());
        $this->addInteger(self::RATING, 'Hodnocení')
            ->setHtmlAttribute('placeholder', 'Hodnocení');

        $this->setDefaults($defaults);
    }
}
