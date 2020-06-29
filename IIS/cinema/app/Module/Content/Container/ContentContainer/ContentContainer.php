<?php declare(strict_types = 1);

namespace App\Module\Content\Container\ContentContainer;

use App\Module\Content\Entity\Content;
use App\Module\Content\Repository\GenreRepository;
use App\Module\Content\Repository\TypeRepository;
use Doctrine\Common\Collections\ArrayCollection;
use Nette\Forms\Container;

/**
 */
class ContentContainer extends Container
{
    private const
        TITLE = 'title',
        DESCRIPTION = 'description',
        TYPE = 'type',
        GENRES = 'genres',
        RATING = 'rating',
        LENGTH = 'length',
        PERFORMERS = 'performers',
        IMAGE = 'image';

    /** @var GenreRepository */
    private $genreRepository;

    /** @var TypeRepository */
    private $typeRepository;

    public function __construct(GenreRepository $genreRepository, TypeRepository $typeRepository, ?Content $content = null)
    {
        $this->genreRepository = $genreRepository;
        $this->typeRepository = $typeRepository;

        $this->addText(self::TITLE, 'Název')
            ->setRequired();
        $this->addTextArea(self::DESCRIPTION, 'Popis')
            ->setRequired();
        $this->addSelect(self::TYPE, 'Typ', $typeRepository->getPairs())
            ->setRequired();
        $this->addMultiSelect(self::GENRES, 'Žánry', $genreRepository->getPairs())
            ->setRequired();
        $this->addInteger(self::RATING, 'Hodnocení')
            ->setRequired();
        $this->addInteger(self::LENGTH, 'Délka [min]')
            ->setRequired();
        $this->addText(self::PERFORMERS, 'Účinkující')
            ->setRequired();
        $this->addImgUpload(self::IMAGE, 'Fotka');

        if ($content) {
            $this->setDefaults([
                self::TITLE => $content->getTitle(),
                self::DESCRIPTION => $content->getDescription(),
                self::TYPE => $content->getType()->getId(),
                self::GENRES => $content->getGenreIds(),
                self::RATING => $content->getRating(),
                self::LENGTH => $content->getLength(),
                self::PERFORMERS => $content->getPerformers(),
            ]);
        }
    }

    public function getValues($returnType = null): ContentData
    {
        $values = parent::getValues('array');

        return new ContentData(
            $values[self::TITLE],
            $values[self::DESCRIPTION],
            $this->typeRepository->getById($values[self::TYPE]),
            new ArrayCollection($this->genreRepository->getByIds($values[self::GENRES])),
            $values[self::RATING],
            $values[self::LENGTH],
            $values[self::PERFORMERS],
            $values[self::IMAGE]
        );
    }
}
